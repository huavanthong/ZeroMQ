// ch7_dealer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <zmq_addon.hpp>

using namespace std::chrono_literals;

static zmq::context_t ctx;

int main()
{
	zmq::socket_t sock0(ctx, zmq::socket_type::pull);
	zmq::socket_t sock1(ctx, zmq::socket_type::pull);
	sock0.connect("tcp://127.0.0.1:5555");
	sock1.connect("tcp://127.0.0.1:5556");

	while (true) {
		// Receive messsage from push socket 0
		zmq::message_t z_in0;
		auto rc0 = sock0.recv(z_in0, zmq::recv_flags::dontwait);
		if(rc0.has_value())
			std::cout << "\n" << z_in0.to_string();

		// Receive messsage from push socket 1
		zmq::message_t z_in1;
		auto rc1 = sock1.recv(z_in0, zmq::recv_flags::dontwait);
		if (rc1.has_value())
			std::cout << "\n" << z_in1.to_string();
	}
}