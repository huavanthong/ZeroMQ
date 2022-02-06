#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <zmq_addon.hpp>

using namespace std::chrono_literals;

static zmq::context_t ctx;

int main()
{
    zmq::socket_t sock(ctx, zmq::socket_type::req);
    sock.connect("tcp://127.0.0.1:5559");

    int request_nbr;
    std::string msg_out = "Hello";
    zmq::message_t z_out(msg_out);
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {

        sock.send(z_out, zmq::send_flags::none);

        zmq::message_t z_in;
        sock.recv(z_in);
        std::cout << "Received reply %d [%s]\n" <<  request_nbr << z_in.to_string();

    }
    sock.close();
    ctx.close();

    return 0;
}