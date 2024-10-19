#include <iostream>
#include <sstream>
#include <string>
#include "sender.hpp"
#include <boost/asio.hpp>

constexpr short multicast_port = 30001;

Sender :: Sender(boost::asio::io_context& io_context,
    const boost::asio::ip::address& multicast_address)
    : endpoint_(multicast_address, multicast_port),
    socket_(io_context, endpoint_.protocol()),
    timer_(io_context),
    messageCount(0)
{
    doSend();
}

void Sender :: doSend()
{
    messageCount++;
    socket_.async_send_to(
        boost::asio::buffer(message), endpoint_,
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec && messageCount < maxMessageCount)
                doTimeout();
        });
}

void Sender::doTimeout()
{
    timer_.expires_after(std::chrono::seconds(1));
    timer_.async_wait(
        [this](boost::system::error_code ec)
        {
            if (!ec)
                doSend();
        });
}