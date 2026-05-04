#pragma once
#include "Order.hpp"
#include "PriceLevel.hpp"
#include <vector>
#include <unordered_map>
#include <list>
#include <functional>
#include <map>
#include <memory>

namespace lobv::business_logic{

    class OrderBook{
    public:

        OrderBook(){}
        bool AddOrder(OrderId id, OrderType type, Side side, Price price, Quantity quantity);
        bool CancelOrder(OrderId order_id);
        std::shared_ptr<PriceLevel> GetOrCreatePriceLevel(Side side, Price price);
        Quantity GetVolumeAtPrice(Side side, Price price);

    private:

        std::unordered_map<OrderId, std::shared_ptr<Order>> _order_map;
        std::map<Price, std::shared_ptr<PriceLevel>, std::greater<Price>> _bids;
        std::map<Price, std::shared_ptr<PriceLevel>, std::less<Price>> _asks;

    };
}