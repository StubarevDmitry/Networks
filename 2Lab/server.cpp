#include "server.hpp"
#include "session.hpp"


Server::Server(boost::asio::io_context& io_context, short port)
    : acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
    doAccept();
}

void Server::doAccept() {
    acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                // Создаем новую сессию
                auto session = std::make_shared<Session>(std::move(socket));

                // Запуск новой сессии
                std::thread([session]() {
                    session->start();
                    }).detach();
            }

            // принять ещё одну сессию
            doAccept();
        });
}