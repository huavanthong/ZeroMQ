#include <iostream>
#include <zmq_addon.hpp>
#include <atomic>
#include <signal.h>
#include <nlohmann/json.hpp>


static zmq::context_t ctx;

static std::atomic_bool b{ false };
void sig_handler(int signum) {
	std::cout << "\n signal interrupt(" << signum << ")";
	b = true;
}

int main()
{
	signal(SIGINT, sig_handler);

	zmq::socket_t sock(ctx, zmq::socket_type::rep);
	sock.bind("tcp://127.0.0.1:5555");

	while (1) {
		zmq::message_t z_in;
		sock.recv(z_in);

		auto jmsg_in = nlohmann::json::parse(z_in.to_string());
		int x = jmsg_in["randint"];
		std::string thread_id = jmsg_in["thread_id"];

		nlohmann::json jmsg_out;
		jmsg_out["result"] = x * x;
		jmsg_out["server"] = true;

		zmq::message_t z_out(jmsg_out.dump());
		sock.send(z_out, zmq::send_flags::none);

		if (b) break;
	}

	sock.close();
	ctx.close();

	std::cout << "\n sockets close and context destroyed.";
}