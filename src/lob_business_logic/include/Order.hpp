#pragma once
#include <chrono>
#include <memory>
#include "Types.hpp"

namespace lobv::business_logic{

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
        std::shared_ptr<PriceLevel> GetParent() const;
        bool SetParent(std::shared_ptr<PriceLevel>);
        bool SetQuantity(Quantity quantity);

        std::weak_ptr<Order> _previous_order{};
        std::weak_ptr<Order> _next_order{};

    private:

        OrderId _order_id;
        OrderType _order_type;
        Side _side;
        Price _price;
        Quantity _quantity;
        std::weak_ptr<PriceLevel> _parent{};
    };

}