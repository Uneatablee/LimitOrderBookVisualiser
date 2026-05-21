#include <random>
#include "MarketDepthDataMock.hpp"

namespace Mocks::UiMocks{

    OrderBookStateSnapshot GenerateMockSnapshot(int market_depth){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> volume_distrib(50, 500);
        std::uniform_int_distribution<> tick_step(1, 3);

        int mid_price = 100;
        int spread = 3;

        int current_ask = mid_price + spread / 2;
        int current_bid = mid_price - spread / 2;

        OrderBookStateSnapshot snap;

        for(int i = 0; i < market_depth; i++){
            snap.asks_price_level_volumes[current_ask] = volume_distrib(gen);
            current_ask += tick_step(gen);

            snap.bids_price_level_volumes[current_bid] = volume_distrib(gen);
            current_bid -= tick_step(gen);
        }

        return snap;
    }

    void MarketDepthMock(std::function<void(OrderBookStateSnapshot)> const& callback, int market_depth){
        OrderBookStateSnapshot state = GenerateMockSnapshot(market_depth);
        callback(state);
    }
}