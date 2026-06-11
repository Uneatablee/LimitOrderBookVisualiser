#pragma once
#include "../Intent/UserIntent.hpp"
#include <memory>
#include "QObject"
#include "MarketDepthDataMock.hpp"
#include "MatchingEngine.hpp"

struct ViewState;
class QTimer;

class ViewModel : public QObject{
    Q_OBJECT
public:
    ViewModel();
    void HandleIntent(const Intents::UserIntent::Intent& intent);
    void UpdateViewState();
    void UpdateViewStateWithMock(const Mocks::UiMocks::OrderBookStateSnapshot& mock);
    void MarketDepthUpdate(unsigned int market_depth);
    void SimulateMarketTick();
    const std::shared_ptr<ViewState> GetState() const;

signals:
    void StateChanged(std::shared_ptr<ViewState> state);

private:
    std::shared_ptr<ViewState> _view_state;
    std::shared_ptr<lobv::business_logic::MatchingEngine> _matching_engine;

    //QT OWNERSHIP
    QTimer* _mock_timer;
};