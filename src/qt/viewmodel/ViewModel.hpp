#pragma once
#include "../Intent/UserIntent.hpp"
#include <memory>
#include "QObject"
#include "MarketDepthDataMock.hpp"
//temporary viewmodel for preliminary ui testing ------>

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
    void StateChanged(const std::shared_ptr<ViewState> state);

private:
    std::shared_ptr<ViewState> _view_state;

    //QT OWNERSHIP
    QTimer* _mock_timer;
};