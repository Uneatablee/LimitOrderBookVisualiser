#include "../include/MatchingEngine.hpp"
#include "../include/OrderBook.hpp"
#include <memory>
#include <iostream>

namespace lobv::business_logic{
    MatchingEngine::MatchingEngine() : _order_book{std::make_unique<OrderBook>()}{}

}