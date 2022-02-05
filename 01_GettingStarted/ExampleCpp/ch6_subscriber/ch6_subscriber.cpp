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
			// #### At old version ZeroMQ, we use setsockopt to set option ####
			// sock.setsockopt(ZMQ_SUBSCRIBE, topic.data(), topic.size());
			// ZMQ_CPP11_DEPRECATED("from 4.7.0, use `set` taking option from zmq::sockopt") 
			// #### Refer: https://github.com/zeromq/cppzmq/issues/403
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