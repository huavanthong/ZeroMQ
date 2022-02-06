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

auto funcA = [](std::string origin_id)
{
    zmq::socket_t sock(ctx, zmq::socket_type::dealer);
    std::string origin = origin_id;
    // Refer: https://github.com/zeromq/cppzmq/issues/456
    sock.set(zmq::sockopt::routing_id, origin);
    sock.connect("tcp://127.0.0.1:5555");

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> normdist(0., 1.);

    std::uniform_int_distribution<int> udist(0, 100);


    while (1) {
        std::cout << "\n DealerA send message to Router ";

        nlohmann::json jmsg;
        jmsg["randvar"] = normdist(mt);

        zmq::message_t z_out(jmsg.dump());
        sock.send(z_out, zmq::send_flags::none);

        std::this_thread::sleep_for(1000ms);
    }
};

auto funcB = [](std::string origin_id)
{
    zmq::socket_t sock(ctx, zmq::socket_type::dealer);
    std::string origin = origin_id;
    // Refer: https://github.com/zeromq/cppzmq/issues/456
    sock.set(zmq::sockopt::routing_id, origin);
    sock.connect("tcp://127.0.0.1:5555");

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> normdist(0., 1.);

    while (1) {
        std::cout << "\n DealerB receive message from Router ";
        std::vector<zmq::message_t> recv_msgs;

        auto res = zmq::recv_multipart(sock, std::back_inserter(recv_msgs));

        for (auto&& msg : recv_msgs)
            std::cout << "\n" << msg.to_string();
    }
};


int main()
{
    std::thread th0 = std::thread(funcA, "strat_A");
    std::thread th1 = std::thread(funcB, "strat_B");
    th0.join();
    th1.join();
}