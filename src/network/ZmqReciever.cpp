#include "ZmqReciever.hpp"
#include <iostream>

namespace lobv::network{

    void ZmqReciever::StartRecieving(){
        _worker = std::jthread{[this](std::stop_token stop_token){

            zmq::context_t context(1);
            zmq::socket_t socket(context, zmq::socket_type::pull);
            socket.bind("tcp://127.0.0.1:5555");

            socket.set(zmq::sockopt::rcvtimeo, 100);

            while(!stop_token.stop_requested()){

                zmq::message_t msg;
                auto res = socket.recv(msg, zmq::recv_flags::none);

                if(res){
                    if(msg.size() != sizeof(lobv::business_logic::OrderMessage)){
                        continue;
                    }

                    lobv::business_logic::OrderMessage order_msg;
                    std::memcpy(&order_msg, msg.data(), sizeof(lobv::business_logic::OrderMessage));

                    {
                        std::lock_guard<std::mutex> lock(*_mutex);
                        _circular_buffer->push_back(order_msg);
                    }
                }
            }
        }};
    }
}