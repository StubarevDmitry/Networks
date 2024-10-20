#include "receiver.hpp"



Receiver::Receiver(boost::asio::io_context& io_context,
    const boost::asio::ip::address& listen_address,
    const boost::asio::ip::address& multicast_address, TemporaryStorage* ts): socket_(io_context), ts(ts){
    // настройка сокета
    boost::asio::ip::udp::endpoint listen_endpoint(listen_address, multicastPort);
    socket_.open(listen_endpoint.protocol());
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(listen_endpoint);

    // присоединение к мультикаст группе
    socket_.set_option(boost::asio::ip::multicast::join_group(multicast_address));

    doReceive();
}

void Receiver::doReceive()
{
    socket_.async_receive_from(
        boost::asio::buffer(data), senderEndpoint_,
        [this](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                ts->putInStorage(senderEndpoint_.address().to_string(), senderEndpoint_.port());
                doReceive();
            }
        });
}
