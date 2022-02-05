#include <iostream>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

int main() 
{
	zmq::socket_t sock(ctx, zmq::socket_type::rep);
	sock.bind("tcp://127.0.0.1:5555");

	while (1) {
		zmq::message_t z_in;
		sock.recv(z_in);

		int x = std::stoi(z_in.to_string());
		x = x * x;
		
		std::string msg_out = std::to_string(x);
		zmq::message_t z_out(msg_out);
		sock.send(z_out, zmq::send_flags::none);
	}
	
}