#include "ViewModel.hpp"
#include "../Intent/UserIntent.hpp"
#include "../viewstate/ViewState.hpp"
#include "../view/include/MarketDepthGraph.hpp"
#include <variant>
#include <QTimer>
#include "OrderBook.hpp"

using namespace Intents::UserIntent;

template<typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

template<typename... Ts>
overload(Ts...) -> overload<Ts...>;

ViewModel::ViewModel(){
    // Inicjalizacja silnika bezpośrednio w konstruktorze
    _matching_engine = std::make_shared<lobv::business_logic::MatchingEngine>();
    _view_state = std::make_shared<ViewState>();

    // Rzeczywisty tick rynkowy - wyciąga dane z ZMQ
    _mock_timer = new QTimer(this);
    connect(_mock_timer, &QTimer::timeout, this, &ViewModel::SimulateMarketTick);
    _mock_timer->start(100);
}

void ViewModel::HandleIntent(const Intent& intent){
    std::visit(
        overload{
            [this](const UpdateDataIntent&){
                // 1. Zleć silnikowi przetworzenie nowych wiadomości z ZMQ
                _matching_engine->ProcessQueue();

                // 2. Pobierz rzeczywisty, aktualny stan OrderBooka
                auto snapshot = _matching_engine-> GetOrderBook()-> GetSnapshot();

                // 3. Przekaż do zaktualizowania widoku
                UpdateViewStateWithMock(snapshot);
            },
            [this](const ChangeMarketDepthIntent& intent){
                MarketDepthUpdate(intent._market_depth);
            },
            [this](const GenerateMarketDepthMockDataIntent& intent){
                // Stary kod mocka (możesz go zostawić do testów wariantów)
                Mocks::UiMocks::MarketDepthMock([this](const Mocks::UiMocks::OrderBookStateSnapshot& snapshot){
                    UpdateViewStateWithMock(snapshot);
                } ,intent._market_depth);
            }
        } , intent);
}

void ViewModel::UpdateViewState(){
    // Pozostaje puste lub do usunięcia, jeśli logika przeszła do UpdateDataIntent
}

// Ta metoda obsługuje teraz zarówno mocki, jak i PRAWDZIWE dane
void ViewModel::UpdateViewStateWithMock(const Mocks::UiMocks::OrderBookStateSnapshot& data){
    _view_state -> asks_price_level_volumes = data.asks_price_level_volumes;
    _view_state -> bids_price_level_volumes = data.bids_price_level_volumes;

    // Powiadom wykres o nowych danych
    emit StateChanged(_view_state);
}

void ViewModel::MarketDepthUpdate(unsigned int market_depth){
}

const std::shared_ptr<ViewState> ViewModel::GetState() const{
    return _view_state;
}

void ViewModel::SimulateMarketTick(){
    // ZMIANA: Zamiast generować mocki, wywołujemy intent aktualizacji rzeczywistych danych
    HandleIntent(Intents::UserIntent::UpdateDataIntent{});
}