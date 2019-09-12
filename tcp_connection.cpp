#include "tcp_connection.hpp"

constexpr uint32_t TCPConnection::buffer_size;

TCPConnection::TCPConnection(
    boost::asio::io_service& io_service
)
: main_socket_(
    io_service
)
{
}

TCPConnection::pointer TCPConnection::create(
    boost::asio::io_service& io_service
)
{
    return pointer(new TCPConnection(io_service));
}

boost::asio::ip::tcp::socket& TCPConnection::socket()
{
    return main_socket_;
}

void TCPConnection::start()
{    
    main_socket_.async_read_some(
        boost::asio::buffer(buffer_, 1000),
        boost::bind(
            &TCPConnection::handle_receive,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}


void TCPConnection::handle_receive(
    const boost::system::error_code& receive_error, 
    std::size_t read_size
)
{
    boost::system::error_code endpoint_error;
    boost::asio::ip::tcp::endpoint remote_endpoint = main_socket_.remote_endpoint(endpoint_error);

    if (receive_error or endpoint_error) // error in receiving or resolving the remote endpoint
    {
        std::cout << receive_error.message() << " | " << endpoint_error.message() << std::endl;
        return;
    }
    else if (read_size <= 2) // not a proper size of a DNS packet
    {
        return;
    }

    std::string sni_name;

    if ( get_sni_from_tls_handshake(buffer_, read_size, sni_name) )
    {
        std::cout << "sni name captured: " << sni_name << "\n";
    }

    boost::system::error_code shutdown_error;

    main_socket_.shutdown(
        boost::asio::ip::tcp::socket::shutdown_both,
        shutdown_error
    );

    main_socket_.close();
}

// void TCPConnection::handle_send(
//     const boost::system::error_code& error_code, 
//     std::size_t
// )
// {
//     if (error_code)
//         std::cout << error_code.message() << std::endl;
// }
