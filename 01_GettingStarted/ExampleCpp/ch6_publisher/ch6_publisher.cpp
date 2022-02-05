#include <iostream>
#include <thread>
#include <chrono>
#include <zmq_addon.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <signal.h>
#include <atomic>

using namespace std::chrono_literals;

static zmq::context_t ctx;

static std::atomic_bool b{ false };
void sig_handler(int signum) {
	std::cout << "\n signal interrupt at client(" << signum << ")";
	b = true;
}

int main()
{
	signal(SIGINT, sig_handler);

	zmq::socket_t sock(ctx, zmq::socket_type::pub);
	sock.bind("tcp://127.0.0.1:5555");

	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<double> normdist(0., 0.1);
	std::uniform_int_distribution<int> udist(0, 100);

	while (1) {
		std::this_thread::sleep_for(200ms);

		std::string name = "";
		nlohmann::json jmsg;
		auto randint = udist(mt);
		if (randint == 0) {
			name = "amd";
			jmsg[name] = normdist(mt);
		}else if(randint == 1){
			name = "intc";
			jmsg[name] = normdist(mt);
		}
		else {
			name = "aapl";
			jmsg[name] = normdist(mt);
		}

		std::cout << "\n" << jmsg.dump();
		
		// topic
		zmq::message_t topic(name);
		sock.send(topic, zmq::send_flags::sndmore);

		// main payload
		zmq::message_t payload(jmsg.dump());
		sock.send(payload, zmq::send_flags::none);
	}
}