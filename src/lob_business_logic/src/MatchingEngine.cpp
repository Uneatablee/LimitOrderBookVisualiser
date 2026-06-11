#include "../include/MatchingEngine.hpp"
#include "../include/OrderBook.hpp"
#include <memory>
#include <iostream>

namespace lobv::business_logic{

    MatchingEngine::MatchingEngine() :
        _order_book{std::make_unique<OrderBook>()},
        _circular_buffer{5000}
    {
        _zmq_receiver = std::make_unique<lobv::network::ZmqReciever>(&_circular_buffer, &_buffer_mutex);
        _zmq_receiver->StartRecieving();
    }

    void MatchingEngine::ProcessQueue() {
        std::vector<lobv::business_logic::OrderMessage> local_messages;

        {
            std::lock_guard<std::mutex> lock(_buffer_mutex);
            while(!_circular_buffer.empty()) {
                local_messages.push_back(_circular_buffer.front());
                _circular_buffer.pop_front();
            }
        }

        for(const auto& msg : local_messages) {
            std::cout << "Matchowanie: ID=" << msg.id << " Cena=" << msg.price << " Ilosc=" << msg.Quantity << "\n";
            _order_book->MatchOrder(msg.side, msg.price, msg.Quantity);
        }
    }
}