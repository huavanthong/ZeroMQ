// ch7_router.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

int main()
{
    zmq::socket_t sock(ctx, zmq::socket_type::router);
    sock.bind("tcp://127.0.0.1:5555");

    while (1) {
        zmq::message_t msg;
        std::cout << "\n Router receive message";
        std::vector<zmq::message_t> rcv_msgs;

        auto res = zmq::recv_multipart(sock, std::back_inserter(rcv_msgs));

        if (res.has_value())
            std::cout << "\n-----------------\n true";

        for (auto& msg : rcv_msgs) {
            std::cout << "\n" << msg;
        }
    }
}