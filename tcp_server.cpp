#include "tcp_server.hpp"

TCPServer::TCPServer(boost::asio::io_service& io_service)
:acceptor_(
    io_service, 
    boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address::from_string("129.22.150.52"), 
        443
    )
)
{
    std::cout << "initializing\n";
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
