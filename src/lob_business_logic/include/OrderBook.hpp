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
        PriceLevel* GetOrCreatePriceLevel(Side side, Price price);
        //Quantity GetVolumeAtPrice();

    private:

        std::unordered_map<OrderId, Order*> _order_map;
        std::map<Price, PriceLevel*, std::greater<Price>> _bids;
        std::map<Price, PriceLevel*, std::less<Price>> _asks;

    };
}