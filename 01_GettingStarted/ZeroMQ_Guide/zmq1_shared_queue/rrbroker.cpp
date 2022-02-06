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

            while (1) {
                //  Process all parts of the message
                auto res = zmq::recv_multipart(frontend, std::back_inserter(rcv_msgs));

                if (res.has_value())
                {
                    std::cout << "\Front end receive message\n";
                    for (auto& msg : rcv_msgs) {
                        std::cout << "\n" << msg;
                    }
                }

                //if (!more)
                //    break;      //  Last message part
            }

        }

        if (items[1].revents & ZMQ_POLLIN) {
            while (1) {
                //  Process all parts of the message
                auto res = zmq::recv_multipart(backend, std::back_inserter(rcv_msgs));

                if (res.has_value())
                {
                    std::cout << "\Back end receive message\n";
                    for (auto& msg : rcv_msgs) {
                        std::cout << "\n" << msg;
                    }
                }

                /*
                int more = zmq_msg_more(&message);
                zmq_msg_send(&message, frontend, more ? ZMQ_SNDMORE : 0);
                zmq_msg_close(&message);
                if (!more)
                    break;      //  Last message part
                */
            }
        }
    }
    //  We never get here, but clean up anyhow
    frontend.close();
    backend.close();
    ctx.close();

    return 0;
}