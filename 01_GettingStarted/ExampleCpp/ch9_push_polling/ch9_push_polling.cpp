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
	zmq::socket_t sock0(ctx, zmq::socket_type::push);
	zmq::socket_t sock1(ctx, zmq::socket_type::push);
	sock0.bind("tcp://127.0.0.1:5555");
	sock1.bind("tcp://127.0.0.1:5556");

	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<double> normdist(0., 1.);

	while (1) {
		auto randvar = normdist(mt);

		if (randvar > 0.) {
			nlohmann::json jmsg;
			jmsg["for sock0"] = randvar;
			zmq::message_t z_out(jmsg.dump());
			sock0.send(z_out, zmq::send_flags::none);
		}
		else {
			nlohmann::json jmsg;
			jmsg["for sock1"] = randvar;
			zmq::message_t z_out(jmsg.dump());
			sock0.send(z_out, zmq::send_flags::none);

		}

		std::this_thread::sleep_for(500ms);
	}
}