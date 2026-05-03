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

    PriceLevel* Order::GetParent() const {
        return _parent;
    }

    bool Order::SetParent(PriceLevel* price_level){
        _parent = price_level;
        return true;
    }
}