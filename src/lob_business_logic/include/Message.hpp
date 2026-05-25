#pragma once
#include "Types.hpp"
#include <vector>

namespace lobv::business_logic{

    class IMessage{};

    class TradeMatched : IMessage{
    public:
        TradeMatched(OrderId maker, OrderId taker, Quantity quantity, Price price)
            : _maker_id{maker}, _taker_id{taker}, _traded_quantity{quantity}, _price{price} {}

        OrderId _maker_id;
        OrderId _taker_id;
        Quantity _traded_quantity;
        Price _price;
    };

    struct Trades{
        std::vector<TradeMatched> _trades;
        Quantity _remaining_order_quantity;
    };
}