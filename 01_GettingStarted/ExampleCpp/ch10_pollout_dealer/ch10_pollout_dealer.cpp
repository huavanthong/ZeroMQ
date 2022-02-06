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
    std::string origin = "strat_A";
    // Refer: https://github.com/zeromq/cppzmq/issues/456
    sock.set(zmq::sockopt::routing_id, origin);
    sock.connect("tcp://127.0.0.1:5555");

    zmq::pollitem_t items[] = {
        {sock, 0, ZMQ_POLLOUT, 0}
    };

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> normdist(0., 1.);

    size_t iter = 0;
    std::chrono::duration<int, std::kilo> ks(0);

    while (1) {
        // auto rc = zmq::poll(items, 1, 0);
        // -1: infinite blocking; +ve value is block ms; 0 return immediately.
        auto rc = zmq::poll(items, 1, ks);
        if (iter++ % 100000)
            std::cout << rc;

        if (items[0].revents & ZMQ_POLLOUT) {
            nlohmann::json jmsg;
            jmsg["randvar"] = normdist(mt);
            jmsg["iter"] = iter;
            zmq::message_t z_out(jmsg.dump());
            sock.send(z_out, zmq::send_flags::none);
        }


        std::this_thread::sleep_for(1000ms);
    }
}