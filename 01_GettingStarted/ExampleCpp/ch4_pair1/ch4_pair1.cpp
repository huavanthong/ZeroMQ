#include <iostream>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

int main()
{
	zmq::socket_t sock(ctx, zmq::socket_type::pair);
	sock.bind("tcp://127.0.0.1:5555");

	while (1) {
		zmq::message_t z_in;
		sock.recv(z_in);
		std::cout << "\n" << z_in.to_string();

		zmq::message_t z_out("ok", 2);
		sock.send(z_out, zmq::send_flags::none);
	}
	sock.close();
	ctx.close();
}