#include <iostream>
#include <thread>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

auto func = [](std::string thread_id)
{
	zmq::socket_t sock(ctx, zmq::socket_type::pull);
	sock.connect("tcp://127.0.0.1:5555");

	while (1) {
		zmq::message_t z_in;
		sock.recv(z_in);

		std::cout
			<< "\n" << thread_id << " : " << z_in.to_string();

	}
};

int main()
{
	std::thread th0 = std::thread(func, "th0");
	std::thread th1 = std::thread(func, "th1");
	std::thread th2 = std::thread(func, "th2");

	th0.join();
	th1.join();
	th2.join();

}