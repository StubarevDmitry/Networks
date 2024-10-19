#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>

class Sender
{
public:
    Sender(boost::asio::io_context& io_context,
        const boost::asio::ip::address& multicast_address);

private:
    void doSend();

    void doTimeout();

    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::steady_timer timer_;
    int messageCount;
    int maxMessageCount = 1000;
    std::string message;
};