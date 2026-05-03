#pragma once
#include <chrono>

namespace lobv::business_logic{

    enum class OrderType{
        GoodTilCancelled,
        ImmediateOrCancel,
        FillOrKill,
        GoodForDay
    };

    enum class Side{
        BuySide,
        SellSide
    };

    using Price = int64_t;
    using OrderId = uint64_t;
    using Quantity = uint64_t;
    class PriceLevel;

    class Order{
    public:

        Order(OrderId id, OrderType type, Side side, Price price, Quantity quantity) :
            _order_id(id), _order_type(type), _side(side), _price(price), _quantity(quantity)
            {}

        OrderId GetOrderId() const;
        OrderType GetOrderType() const;
        Side GetSide() const;
        Price GetPrice() const;
        Quantity GetQuantity() const;
        PriceLevel* GetParent() const;
        bool SetParent(PriceLevel*);

        Order* _previous_order = nullptr;
        Order* _next_order = nullptr;

    private:

        OrderId _order_id;
        OrderType _order_type;
        Side _side;
        Price _price;
        Quantity _quantity;
        PriceLevel* _parent;
    };

}