#ifndef TCP_SERVER_
#define TCP_SERVER_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

#include "tcp_connection.hpp"

class TCPServer
{
    public:
        TCPServer(boost::asio::io_service&);

    private:
        void start_accept();

        void handle_accept(
            TCPConnection::pointer,
            const boost::system::error_code&
        );

        boost::asio::ip::tcp::acceptor acceptor_;
};
#endif
