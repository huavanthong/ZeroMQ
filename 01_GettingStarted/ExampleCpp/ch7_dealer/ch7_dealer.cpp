// ch7_dealer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <nlohmann/json.hpp>
#include <zmq_addon.hpp>

using namespace std::chrono_literals;

static zmq::context_t ctx;

int main()
{
    zmq::socket_t sock(ctx, zmq::socket_type::dealer);
    sock.connect("tcp://127.0.0.1:5555");

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> normdist(0., 1.);

    std::uniform_int_distribution<int> udist(0, 100);

    while (1) {

        nlohmann::json jmsg;
        jmsg["randvar"] = normdist(mt);

        zmq::message_t z_out(jmsg.dump());
        sock.send(z_out, zmq::send_flags::none);

        std::this_thread::sleep_for(1000ms);
    }
}