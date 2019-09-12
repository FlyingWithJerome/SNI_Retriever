#ifndef SERVER_MAIN_
#define SERVER_MAIN_

#include <thread>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "tcp_server.hpp"
#include "keyboard_interruption.hpp"

int main(int argc, char **argv)
{
    keyboard_interruption_register();

    boost::asio::io_service service;
    boost::thread_group thread_pool_;
    TCPServer tcp_server(service);

    unsigned int number_of_threads = 
    std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 16;

    for(unsigned int index = 0; index < number_of_threads - 1; index++)
    {
        thread_pool_.create_thread(
            [&service](){ service.run(); }
        );
    }

    try
    {
        thread_pool_.join_all();
        service.stop();

        return EXIT_SUCCESS;
    }
    catch (const KeyboardInterruption& k)
    {
        std::cout << k.what() << std::endl;

        service.stop();
        thread_pool_.join_all();

        return EXIT_SUCCESS;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;

        service.stop();
        thread_pool_.join_all();

        return EXIT_FAILURE;
    }
}

#endif
