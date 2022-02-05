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

auto func = [](std::string thread_id) {

	zmq::socket_t sock(ctx, zmq::socket_type::req);
	sock.connect("tcp://127.0.0.1:5555");

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> udist(0, 100);


	while (1) {
		nlohmann::json jmsg;
		jmsg["randint"] = udist(mt);
		jmsg["thread_id"] = thread_id;
		zmq::message_t z_out(jmsg.dump());
		sock.send(z_out, zmq::send_flags::none);

		zmq::message_t z_in;
		sock.recv(z_in);
		std::cout
			<< "\n" << thread_id
			<< " sending: " << jmsg.dump()
			<< " received " << z_in.to_string();

		std::this_thread::sleep_for(500ms);

	}

	sock.close();
};

int main()
{
	signal(SIGINT, sig_handler);
	std::thread th0 = std::thread(func, "thread_0");
	std::thread th1 = std::thread(func, "thread_1");

	th0.join();
	th1.join();

}