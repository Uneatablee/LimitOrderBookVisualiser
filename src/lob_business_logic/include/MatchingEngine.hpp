#pragma once
#include <memory>
#include "Types.hpp"
#include "Message.hpp"
#include "Order.hpp"
#include <vector>

namespace lobv::business_logic{

    class OrderBook;

    class MatchingEngine{
    public:
        MatchingEngine();

    private:
        std::unique_ptr<OrderBook> _order_book;

    };
}