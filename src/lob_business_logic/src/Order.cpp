#include "../include/Order.hpp"
#include "../include/PriceLevel.hpp"

namespace lobv::business_logic{
    OrderId Order::GetOrderId() const {
        return _order_id;
    }

    OrderType Order::GetOrderType() const {
        return _order_type;
    }

    Side Order::GetSide() const {
        return _side;
    }

    Price Order::GetPrice() const {
        return _price;
    }

    Quantity Order::GetQuantity() const {
        return _quantity;
    }

    std::shared_ptr<PriceLevel> Order::GetParent() const {
        return _parent.lock();
    }

    bool Order::SetParent(std::shared_ptr<PriceLevel> price_level){
        _parent = price_level;
        return true;
    }

    bool Order::SetQuantity(Quantity quantity){
        if(quantity <= 0) return false;
        _quantity = quantity;
        return true;
    }
}