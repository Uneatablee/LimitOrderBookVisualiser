#include "ViewModel.hpp"
#include "../Intent/UserIntent.hpp"
#include "../viewstate/ViewState.hpp"
#include "../view/include/MarketDepthGraph.hpp"
#include <variant>
#include <QTimer>

using namespace Intents::UserIntent;

template<typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

template<typename... Ts>
overload(Ts...) -> overload<Ts...>;

ViewModel::ViewModel(){
    _view_state = std::make_shared<ViewState>();

    //Temporary Market tick simulation
    _mock_timer = new QTimer(this);
    connect(_mock_timer, &QTimer::timeout, this, &ViewModel::SimulateMarketTick);
    _mock_timer->start(100);
}

void ViewModel::HandleIntent(const Intent& intent){
    std::visit(
        overload{
            [this](const UpdateDataIntent&){
                UpdateViewState();
            },
            [this](const ChangeMarketDepthIntent& intent){
                MarketDepthUpdate(intent._market_depth);
            },
            [this](const GenerateMarketDepthMockDataIntent& intent){
                Mocks::UiMocks::MarketDepthMock([this](const Mocks::UiMocks::OrderBookStateSnapshot& snapshot){
                    UpdateViewStateWithMock(snapshot);
                } ,intent._market_depth);
            }
        } , intent);
}

void ViewModel::UpdateViewState(){
}

void ViewModel::UpdateViewStateWithMock(const Mocks::UiMocks::OrderBookStateSnapshot& mock){
    _view_state -> asks_price_level_volumes = mock.asks_price_level_volumes;
    _view_state -> bids_price_level_volumes = mock.bids_price_level_volumes;

    emit StateChanged(_view_state);
}

void ViewModel::MarketDepthUpdate(unsigned int market_depth){
}

const std::shared_ptr<ViewState> ViewModel::GetState() const{
    return _view_state;
}

//Later to Change UserIntent to ModelEvent
void ViewModel::SimulateMarketTick(){
    HandleIntent(Intents::UserIntent::GenerateMarketDepthMockDataIntent{40});
}



