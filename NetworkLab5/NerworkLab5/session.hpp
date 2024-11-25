#ifndef Session_hpp
#define Session_hpp

#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include "proxyConst.hpp"
#include "SocketWrapper.hpp"

using boost::asio::ip::tcp;

namespace Server {
    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(boost::asio::io_context& io_context, tcp::socket socket)
            : clientSocket(std::make_shared<SocketWrapper>(io_context)),
            hostSocket(std::make_shared<SocketWrapper>(io_context)),
            resolver_(io_context) {
            clientSocket->getSocket() = std::move(socket);
        }

        void start();

    private:
        // приветствие
        void readGreeting();
        void choosingAuthenticationMethod();

        // запрос о получении данных
        void readRequest();
        void createConnectionWithIPV4Host(tcp::endpoint& endpoints);
        void sendServerResponse(bool success, bool isIP, const std::string& destinationAddress, uint16_t port);
        void connectToDomain(std::string domainName, short port);

        // перекидка данных
        void startDataTransfer();
        void sendDataToHost(size_t len);
        void sendDataToClient(size_t len);
        void recvDataFromHost();
        void recvDataFromClient();

        //закрытие
        void closeConnection();

        //резолвер
        boost::asio::ip::tcp::resolver resolver_;

        //обертка над сокетом и буфером клиента
        std::shared_ptr<SocketWrapper> clientSocket;
        
        //обертка над сокетом и буфером хоста
        std::shared_ptr<SocketWrapper> hostSocket;
    };
}

#endif