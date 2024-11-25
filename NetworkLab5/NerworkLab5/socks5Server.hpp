#pragma once
#include <boost/asio.hpp>


using boost::asio::ip::tcp;

namespace Server {
    class Socks5Server {
    public:
        Socks5Server(boost::asio::io_context& io_context, short port);

    private:
        void doAccept();
        boost::asio::io_context& io_context_;
        tcp::acceptor acceptor;
    };
}