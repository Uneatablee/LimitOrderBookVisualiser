#pragma once
#include "Order.hpp"
#include "PriceLevel.hpp"
#include "Message.hpp"
#include <vector>
#include <unordered_map>
#include <list>
#include <functional>
#include <map>
#include <memory>

#include "MarketDepthDataMock.hpp"

namespace lobv::business_logic{

    class OrderBook{
    public:

        OrderBook(){}
        bool AddOrder(OrderId id, OrderType type, Side side, Price price, Quantity quantity);
        bool CancelOrder(OrderId order_id);
        std::shared_ptr<PriceLevel> GetPriceLevel(Side side, Price price);
        std::shared_ptr<PriceLevel> CreatePriceLevel(Side side, Price price);
        Quantity GetVolumeAtPrice(Side side, Price price);
        bool CanMatch(Side side, Price price);
        Trades MatchOrder(Side side, Price price, Quantity quantity);
        Trades MatchPriceLevel(Price price, Quantity quantity, std::shared_ptr<PriceLevel> price_level);
        Price GetLowestAsk();
        Price GetHighestBid();

        //temporary solution
        Mocks::UiMocks::OrderBookStateSnapshot GetSnapshot();

    private:

        std::unordered_map<OrderId, std::shared_ptr<Order>> _order_map;
        std::map<Price, std::shared_ptr<PriceLevel>, std::greater<Price>> _bids;
        std::map<Price, std::shared_ptr<PriceLevel>, std::less<Price>> _asks;

    };
}