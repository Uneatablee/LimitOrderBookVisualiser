#pragma once
#include "Types.hpp"
#include <vector>

namespace lobv::business_logic{

    class IMessage{};

    class TradeMatchedMessage : IMessage{
    public:
        TradeMatchedMessage(OrderId maker, OrderId taker, Quantity quantity, Price price)
            : _maker_id{maker}, _taker_id{taker}, _traded_quantity{quantity}, _price{price} {}

        OrderId _maker_id;
        OrderId _taker_id;
        Quantity _traded_quantity;
        Price _price;
    };

    struct Trades{
        std::vector<TradeMatchedMessage> _trades;
        Quantity _remaining_order_quantity;
    };

    #pragma pack(push, 1)
    struct OrderMessage{
        OrderId id;
        OrderType order_type;
        Side side;
        Price price;
        Quantity Quantity;
    };
    #pragma pack(pop)
}