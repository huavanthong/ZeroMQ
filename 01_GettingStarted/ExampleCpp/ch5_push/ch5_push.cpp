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

	zmq::socket_t sock(ctx, zmq::socket_type::push);
	sock.bind("tcp://127.0.0.1:5555");

	size_t iter = 0;

	for (;;) {
		iter++;
		nlohmann::json jmsg;
		jmsg["iter"] = iter;

		zmq::message_t z_out(jmsg.dump());
		sock.send(z_out, zmq::send_flags::none);

		std::this_thread::sleep_for(500ms);

	}
}