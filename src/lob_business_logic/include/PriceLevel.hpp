#pragma once
#include <list>

namespace lobv::business_logic{
class PriceLevel{
    public:

        PriceLevel(Price price, Quantity quantity) : _price(price), _quantity(quantity){}
        Price _price;
        Quantity _quantity;
        std::list<Order*> _orders_queue;
    };
}