#include "../include/OrderBook.hpp"

namespace lobv::business_logic{

    PriceLevel* OrderBook::GetOrCreatePriceLevel(Side side, Price price){

        if(side == Side::BuySide){
            if(_bids.empty()){
                auto new_level =  new PriceLevel{price, 0};
                _bids[price] = new_level;
                return new_level;
            }

            auto level = _bids.find(price);
            if(level != _bids.end()){
                return _bids[price];
            }
            else
            {
                return new PriceLevel{price, 0};
            }
        }
        else
        {
            if(_asks.empty()) return new PriceLevel{price, 0};

            auto level = _asks.find(price);
            if(level != _asks.end()){
                return _asks[price];
            }
            else
            {
                return new PriceLevel{price, 0};
            }
        }
    }

    bool OrderBook::AddOrder(OrderId id, OrderType type, Side side, Price price, Quantity quantity){

        PriceLevel* level = GetOrCreatePriceLevel(side, price);
        Order* new_order = new Order{id, type, side, price, quantity};

        new_order -> SetParent(level);
        new_order -> _previous_order = level -> _orders_queue.back();

        level -> _orders_queue.back() -> _next_order = new_order;
        level -> _orders_queue.push_back(new_order);
        level -> _quantity += quantity;

        _order_map[id] = new_order;
        return true;
    }

    bool OrderBook::CancelOrder(OrderId order_id){
        auto order = _order_map[order_id];

        auto price_level = order -> GetParent();
        price_level -> _quantity -= order -> GetQuantity();

        if(order -> _previous_order != nullptr && order -> _next_order != nullptr){
            order -> _previous_order -> _next_order = order -> _next_order;
        }
        else if(order -> _next_order == nullptr){
            order -> _previous_order -> _next_order = nullptr;
        }
        else
        {
            order -> _next_order -> _previous_order = nullptr;
        }

    }
}