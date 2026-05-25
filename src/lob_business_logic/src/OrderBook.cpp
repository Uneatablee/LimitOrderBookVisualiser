#include "../include/OrderBook.hpp"
#include <iostream>
#include <algorithm>

namespace lobv::business_logic{

    std::shared_ptr<PriceLevel> OrderBook::GetPriceLevel(Side side, Price price){

        if(side == Side::BuySide){
            auto level = _bids.find(price);
            if(level != _bids.end()){
                return level -> second;
            }
        }
        else{
            auto level = _asks.find(price);
            if(level != _asks.end()){
                return level -> second;
            }
        }

        return nullptr;
    }

    std::shared_ptr<PriceLevel> OrderBook::CreatePriceLevel(Side side, Price price){

        auto new_level = std::make_shared<PriceLevel>(price, 0);

        if(side == Side::BuySide){
            _bids[price] = new_level;
        }
        else{
            _asks[price] = new_level;
        }

        return new_level;
    }

    bool OrderBook::AddOrder(OrderId id, OrderType type, Side side, Price price, Quantity quantity){

        std::shared_ptr<PriceLevel> level = GetPriceLevel(side, price);
        if(!level){
            level = CreatePriceLevel(side, price);
        }

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

        //refactor later - temporary solution with high overhead
        auto list_it = std::ranges::find_if(price_level -> _orders_queue,
            [order_id](const std::weak_ptr<Order>& weak_order){
                if(auto shared_order = weak_order.lock()){
                    return shared_order -> GetOrderId() == order_id;
                }
                return false;
            }
        );

        if(list_it != price_level -> _orders_queue.end()){
            price_level -> _orders_queue.erase(list_it);
        }

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

    Price OrderBook::GetLowestAsk(){
        if(_asks.empty()) return 0;
        return _asks.begin() -> first;
    }

    Price OrderBook::GetHighestBid(){
        if(_bids.empty()) return 0;
        return _bids.begin() -> first;
    }

    bool OrderBook::CanMatch(Side side, Price price){
        if(side == Side::BuySide){
            auto lowest_ask = GetLowestAsk();
            if(price >= lowest_ask) return true;
        }
        else{
            auto highest_bid = GetHighestBid();
            if(price <= highest_bid) return true;
        }

        return false;
    }

    Trades OrderBook::MatchOrder(Side side, Price price, Quantity quantity){

        Quantity remaining_quantity = quantity;
        Trades trades;

        if(side == Side::BuySide){

            auto lowest_ask = GetLowestAsk();
            while(price >= lowest_ask){

                auto price_level = GetPriceLevel(Side::SellSide, lowest_ask);

                if(!price_level){
                    break;
                }

                trades = MatchPriceLevel(price, remaining_quantity, price_level);
                remaining_quantity = trades._remaining_order_quantity;

                if(!remaining_quantity){
                    break;
                }

                lowest_ask = GetLowestAsk();
            }
        }
        else{

            auto highest_bid = GetHighestBid();

            while(price <= highest_bid){

                auto price_level = GetPriceLevel(Side::BuySide, highest_bid);

                if(!price_level){
                    break;
                }

                trades = MatchPriceLevel(price, remaining_quantity, price_level);
                remaining_quantity = trades._remaining_order_quantity;

                if(!remaining_quantity){
                    break;
                }

                highest_bid = GetHighestBid();
            }
        }

        //temporary AddOrder for testing
        if(remaining_quantity > 0){
            AddOrder(9, OrderType::GoodForDay, side, price, remaining_quantity);
        }

        return trades;
    }

    Trades OrderBook::MatchPriceLevel(Price price, Quantity remaining_quantity, std::shared_ptr<PriceLevel> price_level){

        Trades trades{};

        bool filling = true;
        while(filling)
        {
            if(price_level -> _quantity == 0){
                break;
            }

            if(auto current_order = price_level -> _orders_queue.front().lock()){
                auto current_order_quantity = current_order -> GetQuantity();

                int quantity_difference = current_order_quantity - remaining_quantity;
                if(quantity_difference > 0){

                    //trade messages should be pushed into ring buffer, avoiding copying or
                    //any memory allocations, vector is only a temp solution.
                    trades._trades.push_back({current_order -> GetOrderId(), 0, remaining_quantity, price});
                    current_order -> SetQuantity(quantity_difference);
                    price_level -> _quantity -= remaining_quantity;
                    remaining_quantity = 0;
                    trades._remaining_order_quantity = 0;
                    filling = false;
                }
                else{

                    auto trade_quantity = std::min(current_order_quantity, remaining_quantity);
                    trades._trades.push_back({current_order -> GetOrderId(), 0, trade_quantity, price});
                    CancelOrder(current_order -> GetOrderId());
                    remaining_quantity -= trade_quantity;
                    trades._remaining_order_quantity = remaining_quantity;
                }
            }
            else
            {
                break;
            }
        }

        return trades;
    }
}