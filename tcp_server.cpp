#include "tcp_server.hpp"

TCPServer::TCPServer(
    boost::asio::io_service& io_service,
    const int& port_num
)
:acceptor_(
    io_service, 
    boost::asio::ip::tcp::endpoint(
        boost::asio::ip::tcp::v4(),
        port_num
    )
)
{
    start_accept();
}

void TCPServer::start_accept()
{
    TCPConnection::pointer new_connection = TCPConnection::create(
        acceptor_.get_io_service()
    );

    acceptor_.async_accept(
        new_connection.get()->socket(),
        boost::bind(
            &TCPServer::handle_accept,
            this,
            new_connection,
            boost::asio::placeholders::error
        )
    );
}

void TCPServer::handle_accept(
    TCPConnection::pointer new_connection_copy,
    const boost::system::error_code& error
)
{
    if (!error)
    {
      new_connection_copy.get()->start();
      start_accept();
    }
}
