#pragma once
#include <map>
#include <functional>

namespace Mocks::UiMocks{
    struct OrderBookStateSnapshot{
        std::map<int, int, std::greater<int>> bids_price_level_volumes;
        std::map<int, int, std::less<int>> asks_price_level_volumes;
    };

    OrderBookStateSnapshot GenerateMockSnapshot(int market_depth);
    void MarketDepthMock(std::function<void(const OrderBookStateSnapshot)> const& callback, int market_depth);
};