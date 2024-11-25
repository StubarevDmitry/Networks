#include "socks5Server.hpp"
#include "session.hpp"

using namespace Server;

Socks5Server::Socks5Server(boost::asio::io_context& io_context, short port)
    : acceptor(io_context, tcp::endpoint(tcp::v4(), port)), io_context_(io_context){
    doAccept();
}

void Socks5Server::doAccept() {
    acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                // Создаем новую сессию
                std::cout << "сессия создана" << std::endl;
                auto session = std::make_shared<Session>(io_context_, std::move(socket));
                session->start();
            }

            // принять ещё одну сессию
            doAccept();
        });
}
