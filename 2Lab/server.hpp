#pragma once
#include <boost/asio.hpp>


using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port);

private:
    void doAccept();
    tcp::acceptor acceptor;
};

