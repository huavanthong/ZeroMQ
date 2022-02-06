// ch7_dealer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <zmq_addon.hpp>
#include <thread>

using namespace std::chrono_literals;

static zmq::context_t ctx;


auto clean = []()
{
	zmq::socket_t sock0(ctx, zmq::socket_type::pull);
	zmq::socket_t sock1(ctx, zmq::socket_type::pull);
	sock0.connect("tcp://127.0.0.1:5555");
	sock1.connect("tcp://127.0.0.1:5556");

	zmq::pollitem_t items[] = {
		{sock0, 0, ZMQ_POLLIN, 0},
		{sock1, 0, ZMQ_POLLIN, 0},
	};

	std::chrono::duration<int, std::kilo> ks(-1);

	while (true) {
		// int rc = zmq::poll(items, 2, -1); 
		// Issue: 'zmq::poll': from 4.8.0, use poll taking std::chrono::duration instead of long	ch9_pull_polling
		// Solution: how to use chrono
		// Refer: 
		//      https://en.cppreference.com/w/cpp/chrono/duration/duration
		int rc = zmq::poll(items, 2, ks);

		if (items[0].revents) {
			zmq::message_t z_in0;
			auto rc0 = sock0.recv(z_in0);
			std::cout << "\n" << z_in0.to_string();
		}

		if (items[1].revents) {
			zmq::message_t z_in1;
			auto rc1 = sock1.recv(z_in1);
			std::cout << "\n" << z_in1.to_string();
		}

		std::this_thread::sleep_for(1ms);
	}
};


int main()
{
	std::thread th0 = std::thread(clean);

	th0.join();

}