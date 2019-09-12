#ifndef TCP_CONNECTION_
#define TCP_CONNECTION_

#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "tls_parser.hpp"

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
    public:
        typedef boost::shared_ptr<TCPConnection> pointer;

        static pointer create(
            boost::asio::io_service&
        );

        boost::asio::ip::tcp::socket& socket();

        void start();

        static constexpr uint32_t buffer_size = 1000;

    private:
        TCPConnection(
            boost::asio::io_service&
        );

        void handle_receive(
            const boost::system::error_code&, 
            std::size_t
        );

        boost::asio::ip::tcp::socket main_socket_;

        uint8_t buffer_[buffer_size];        
};
#endif 
