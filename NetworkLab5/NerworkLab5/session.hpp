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
        // �����������
        void readGreeting();
        void choosingAuthenticationMethod();

        // ������ � ��������� ������
        void readRequest();
        void createConnectionWithIPV4Host(tcp::endpoint& endpoints);
        void sendServerResponse(bool success, bool isIP, const std::string& destinationAddress, uint16_t port);
        void connectToDomain(std::string domainName, short port);

        // ��������� ������
        void startDataTransfer();
        void sendDataToHost(size_t len);
        void sendDataToClient(size_t len);
        void recvDataFromHost();
        void recvDataFromClient();

        //��������
        void closeConnection();

        //��������
        boost::asio::ip::tcp::resolver resolver_;

        //������� ��� ������� � ������� �������
        std::shared_ptr<SocketWrapper> clientSocket;
        
        //������� ��� ������� � ������� �����
        std::shared_ptr<SocketWrapper> hostSocket;
    };
}

#endif