#include "../include/OrderBook.hpp"
#include <iostream>

namespace lobv::business_logic{

    std::shared_ptr<PriceLevel> OrderBook::GetOrCreatePriceLevel(Side side, Price price){

        if(side == Side::BuySide){
            auto level = _bids.find(price);
            if(level != _bids.end()){
                return level -> second;
            }

            auto new_level =  std::make_shared<PriceLevel>(price, 0);
            _bids[price] = new_level;
            return new_level;
        }
        else{
            auto level = _asks.find(price);
            if(level != _asks.end()){
                return level -> second;
            }

            auto new_level =  std::make_shared<PriceLevel>(price, 0);
            _asks[price] = new_level;
            return new_level;
        }
    }

    bool OrderBook::AddOrder(OrderId id, OrderType type, Side side, Price price, Quantity quantity){

        std::shared_ptr<PriceLevel> level = GetOrCreatePriceLevel(side, price);
        std::shared_ptr<Order> new_order = std::make_shared<Order>(id, type, side, price, quantity);

        new_order -> SetParent(level);

        if(!level -> _orders_queue.empty()){
            new_order -> _previous_order = level -> _orders_queue.back();
        }

        if(!level -> _orders_queue.empty()){
            if (std::shared_ptr<Order> last_order = level->_orders_queue.back().lock()) {
                last_order -> _next_order = new_order;
            }
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

        std::shared_ptr<Order> previous_ord = order -> _previous_order.lock();
        std::shared_ptr<Order> next_ord = order -> _next_order.lock();

        if(previous_ord){
            previous_ord -> _next_order = next_ord;
        }

        if(next_ord){
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