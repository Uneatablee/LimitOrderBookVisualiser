#include "../include/OrderBook.hpp"
#include <iostream>

namespace lobv::business_logic{

    PriceLevel* OrderBook::GetOrCreatePriceLevel(Side side, Price price){

        if(side == Side::BuySide){
            auto level = _bids.find(price);
            if(level != _bids.end()){
                return _bids[price];
            }

            auto new_level =  new PriceLevel{price, 0};
            _bids[price] = new_level;
            return new_level;
        }
        else{
            auto level = _asks.find(price);
            if(level != _asks.end()){
                return _asks[price];
            }

            auto new_level =  new PriceLevel{price, 0};
            _asks[price] = new_level;
            return new_level;
        }
    }

    bool OrderBook::AddOrder(OrderId id, OrderType type, Side side, Price price, Quantity quantity){

        PriceLevel* level = GetOrCreatePriceLevel(side, price);
        Order* new_order = new Order{id, type, side, price, quantity};

        new_order -> SetParent(level);

        if(!level -> _orders_queue.empty()){
            new_order -> _previous_order = level -> _orders_queue.back();
        }

        if(!level -> _orders_queue.empty()){
            level -> _orders_queue.back() -> _next_order = new_order;
        }

        level -> _orders_queue.push_back(new_order);
        level -> _quantity += quantity;

        _order_map[id] = new_order;
        return true;
    }

    bool OrderBook::CancelOrder(OrderId order_id){

        auto it = _order_map.find(order_id);
        if(it == _order_map.end()){
            return false;
        }
        auto order = it -> second;

        auto previous_ord = order -> _previous_order;
        auto next_ord = order -> _next_order;

        if(previous_ord != nullptr){
            previous_ord -> _next_order = next_ord;
        }

        if(next_ord != nullptr){
            next_ord -> _previous_order = previous_ord;
        }

        auto price_level = order -> GetParent();
        price_level -> _quantity -= order -> GetQuantity();

        if(price_level -> _quantity == 0){
            auto price = order -> GetPrice();
            if(order -> GetSide() == Side::BuySide){
                _bids.erase(price);
            }
            else{
                _asks.erase(price);
            }
        }

        _order_map.erase(it);
        return true;
    }

    Quantity OrderBook::GetVolumeAtPrice(Side side, Price price){
        if(side == Side::BuySide){
            auto it = _bids.find(price);
            if(it != _bids.end()){
                return it -> second -> _quantity;
            }
        }
        else{
            auto it = _asks.find(price);
            if(it != _asks.end()){
                return it -> second -> _quantity;
            }
        }

        return 0;
    }
}