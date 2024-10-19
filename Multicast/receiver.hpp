#include <array>
#include <iostream>
#include <string>
#include "TemporaryStorage.hpp"
#include "boost/asio.hpp"


class Receiver
{
public:
    Receiver(boost::asio::io_context& io_context,
        const boost::asio::ip::address& listen_address,
        const boost::asio::ip::address& multicast_address, TemporaryStorage* ts);

private:
    void doReceive();

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint senderEndpoint_;
    TemporaryStorage* ts;
    short multicastPort = 30001;
    std::array<char, 1024> data;
};