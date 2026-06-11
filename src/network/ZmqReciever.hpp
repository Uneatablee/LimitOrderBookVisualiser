#pragma once
#include <zmq.hpp>
#include <thread>
#include <stop_token>
#include "Message.hpp"
#include <boost/circular_buffer.hpp>
#include <mutex>

namespace lobv::network{

    class ZmqReciever{
    public:
        ZmqReciever(boost::circular_buffer<lobv::business_logic::OrderMessage> *buffer
        ,std::mutex* mtx) : _circular_buffer(buffer), _mutex{mtx}{};
        void StartRecieving();
        void EndRecieving();

    private:

        boost::circular_buffer<lobv::business_logic::OrderMessage>* _circular_buffer;
        std::jthread _worker;
        std::mutex* _mutex;
    };
}