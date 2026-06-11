#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <boost/circular_buffer.hpp>
#include "Types.hpp"
#include "Message.hpp"
#include "Order.hpp"
#include "ZmqReciever.hpp"

namespace lobv::business_logic{

    class OrderBook;

    class MatchingEngine{
    public:
        MatchingEngine();
        void ProcessQueue();

        std::unique_ptr<OrderBook>& GetOrderBook() { return _order_book; }

    private:
        std::unique_ptr<OrderBook> _order_book;

        boost::circular_buffer<lobv::business_logic::OrderMessage> _circular_buffer;

        std::mutex _buffer_mutex;
        std::unique_ptr<lobv::network::ZmqReciever> _zmq_receiver;
    };
}