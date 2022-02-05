// ch1_rep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <zmq_addon.hpp>

using namespace std::chrono_literals;

static zmq::context_t ctx;

int main()
{
    zmq::socket_t sock(ctx, zmq::socket_type::req);
    sock.connect("tcp://127.0.0.1:5555");
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> udist(0, 100);

    while (1) {
        
        std::string msg_out = std::to_string(udist(mt));
        zmq::message_t z_out(msg_out);
        sock.send(z_out, zmq::send_flags::none);
        
        zmq::message_t z_in;
        sock.recv(z_in);
        std::cout
            << "\n sending: " << msg_out
            << "   received " << z_in.to_string();

    }
}