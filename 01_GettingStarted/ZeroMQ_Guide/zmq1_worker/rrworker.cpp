#include <iostream>
#include <thread>
#include <chrono>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

int main()
{
	zmq::socket_t sock(ctx, zmq::socket_type::rep);
	sock.connect("tcp://127.0.0.1:5560");

	while (1) {
		//  Wait for next request from client
		zmq::message_t z_in;
		sock.recv(z_in);
		std::cout << "Received request: [%s]\n" << z_in.to_string();

		//  Do some 'work'
		// std::this_thread::sleep_for(500ms);


		//  Send reply back to client
		std::string msg_out = "World";
		zmq::message_t z_out(msg_out);
		sock.send(z_out, zmq::send_flags::none);
	}
}