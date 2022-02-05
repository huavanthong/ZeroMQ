#include <iostream>
#include <thread>
#include <zmq_addon.hpp>

static zmq::context_t ctx;

int main()
{
	zmq::socket_t sock(ctx, zmq::socket_type::sub);
	sock.connect("tcp://127.0.0.1:5555");

	{
		std::vector<std::string> topics{ "intc", "amd" };
		for (auto&& topic : topics)
			sock.set(zmq::sockopt::subscribe, topic);
	}

	while (1) {
		zmq::message_t topic;
		zmq::message_t payload;

		sock.recv(topic);
		sock.recv(payload);

		std::cout << "\n" << topic.to_string() << " : "
			<< payload.to_string();

	}

}