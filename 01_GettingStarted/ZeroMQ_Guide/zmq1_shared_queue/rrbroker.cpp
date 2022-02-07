//  Simple request-reply broker
#include <iostream>
#include <thread>
#include <chrono>
#include <zmq_addon.hpp>

static zmq::context_t ctx;


int main(void)
{
    //  Prepare our context and sockets
    zmq::socket_t frontend(ctx, zmq::socket_type::router);
    zmq::socket_t backend(ctx, zmq::socket_type::dealer);
    frontend.bind("tcp://*:5559");
    backend.bind("tcp://*:5560");

    //  Initialize poll set
    zmq::pollitem_t items[] = {
        {frontend, 0, ZMQ_POLLIN, 0},
        {backend , 0, ZMQ_POLLIN, 0},
    };
  
    //  Switch messages between sockets
    std::chrono::duration<int, std::kilo> ks(-1);

    while (1) {
        zmq::message_t message;
        std::vector<zmq::message_t> rcv_msgs;

        int rc = zmq::poll(items, 2, ks);

        if (items[0].revents & ZMQ_POLLIN) {

            std::string msg_out;

            while (1) {
                //  Process all parts of the message
                auto res = zmq::recv_multipart(frontend, std::back_inserter(rcv_msgs));

                if (res.has_value())
                {
                    std::cout << "\nBroker receive message from Frontend";

                    msg_out = rcv_msgs.back().to_string();
                    std::cout << "\nMessage out: " << msg_out;

                    int pos = 0;
                    for (auto& msg : rcv_msgs) {
                        std::cout << "\n" << msg << " at pos: " << pos;
                        pos++;
                    }
                }

                zmq::message_t z_out(msg_out);
                std::cout << "\nBroker send message to Backend";
                backend.send(rcv_msgs[0], zmq::send_flags::sndmore);
                backend.send(rcv_msgs[2], zmq::send_flags::none);

            }

        }

        if (items[1].revents & ZMQ_POLLIN) {

            std::string msg_out;

            while (1) {
                //  Process all parts of the message
                auto res = zmq::recv_multipart(backend, std::back_inserter(rcv_msgs));

                if (res.has_value())
                {
                    std::cout << "\Broker receive message from Backend\n";
                    msg_out = rcv_msgs.back().to_string();
                    std::cout << "\nMessage out: " << msg_out;

                    int pos = 0;
                    for (auto& msg : rcv_msgs) {
                        std::cout << "\n" << msg << "at pos: " << pos;
                        pos++;
                    }
                }

                std::cout << "\Broker send message to Frontend\n";
                zmq::message_t z_out(msg_out);
                frontend.send(rcv_msgs[0], zmq::send_flags::sndmore);
                frontend.send(rcv_msgs[2], zmq::send_flags::none);

            }
        }
    }
    //  We never get here, but clean up anyhow
    frontend.close();
    backend.close();
    ctx.close();

    return 0;
}