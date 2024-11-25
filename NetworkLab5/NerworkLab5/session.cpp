#include "session.hpp"

using namespace Server;
using namespace Server::ServerConst;

void Session::start() {
    readGreeting();
}

void Session::readGreeting() {
    auto self(shared_from_this());

    auto buffer = std::make_shared<std::vector<char>>(280);

    clientSocket->getSocket().async_read_some(boost::asio::buffer(*buffer),
        [self, buffer](const boost::system::error_code& error, std::size_t length) {
            if (!error) {
                if (buffer->at(0) == SOCKS5) {
                    std::cout << "��������� �����������" << std::endl;
                    self->choosingAuthenticationMethod();
                }
                else {
                    self->closeConnection();
                }
            }
            else {
                std::cerr << "������ ������: " << error.message() << std::endl;
                self->closeConnection();
            }
        });
}

void Session::choosingAuthenticationMethod() {
    auto self(shared_from_this());

    clientSocket->getSocket().async_write_some(boost::asio::buffer({ char(SOCKS5), char(NO_AUTH) }),
        [self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "����� ��������������" << std::endl;
                self->readRequest();
            }
            else {
                std::cerr << "������ ������: " << ec.message() << std::endl;
                self->closeConnection();
            }
        });
}

void Session::readRequest() {
    auto self(shared_from_this());
    
    auto buffer = std::make_shared<std::vector<char>>(282);
    
    clientSocket->getSocket().async_read_some(boost::asio::buffer(*buffer),
        [self, buffer](const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (!error && buffer->at(0) == SOCKS5 && buffer->at(1) == TCP_CONNECT && buffer->at(2) == RESERVE_BYTE) {
                std::cout << "������� ���������� ������" << std::endl;
                if (buffer->at(3) == IPv4) {
                    // ���� �����
                    uint16_t port;
                    char portParts[2];
                    portParts[0] = buffer->at(8);
                    portParts[1] = buffer->at(9);
                    memcpy(&port, portParts, sizeof(port));
                    port = ntohs(port);

                    // ip �����
                    boost::asio::ip::address_v4::bytes_type ip_address_bytes;
                    char ipParts[4];
                    ipParts[0] = buffer->at(4);
                    ipParts[1] = buffer->at(5);
                    ipParts[2] = buffer->at(6);
                    ipParts[3] = buffer->at(7);
                    memcpy(ip_address_bytes.data(), ipParts, sizeof(ipParts));
                    boost::asio::ip::address_v4 ip(ip_address_bytes);
                    auto endpoints = tcp::endpoint(ip, port);

                    //�������� �������� � ������
                    self->createConnectionWithIPV4Host(endpoints);

                }
                else if (buffer->at(3) == DOMAIN_NAME) {
                    // �������� ��� �����
                    unsigned char len = buffer->at(4);
                    std::string domainName(buffer->data() + 5, buffer->data() + 5 + len);

                    // ���� ����� 
                    uint16_t port;
                    char portPart[2];
                    portPart[0] = buffer->at(5 + len);
                    portPart[1] = buffer->at(6 + len);
                    memcpy(&port, portPart, sizeof(portPart));
                    port = ntohs(port);
                    std::cout << "domainName " << domainName << " : " << port << std::endl;

                    //�������� �������� � ������
                    self->connectToDomain(domainName, port);
                }
                else {
                    std::cout << "������ " << error.message() << std::endl;
                    self->closeConnection();
                }
            }
        }
    );
}

void Session::createConnectionWithIPV4Host(tcp::endpoint& endpoint) {
    auto self(shared_from_this());

    hostSocket->getSocket().async_connect(endpoint,
        [self](const boost::system::error_code& error) {
            if (!error) {
                self->sendServerResponse(true, true,
                    self->clientSocket->getSocket().remote_endpoint().address().to_string(),
                    self->clientSocket->getSocket().remote_endpoint().port());
                self->startDataTransfer();
            }
            else {
                self->sendServerResponse(false, true, 
                    self->clientSocket->getSocket().remote_endpoint().address().to_string(),
                    self->clientSocket->getSocket().remote_endpoint().port());
                std::cout << "������ �� ���������: " << error.message() << std::endl;
                self->closeConnection();
            }
        });
}

void Session::connectToDomain(std::string domainName, short port) {
    resolver_.async_resolve(domainName, std::to_string(port),
        [this, self = shared_from_this(), domainName, port](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::results_type results) {
            if (!err) {
                boost::asio::async_connect(hostSocket->getSocket(), results,
                    [self, this, domainName, port](const boost::system::error_code& err, const boost::asio::ip::tcp::endpoint& /*endpoint*/) {
                        if (!err) {
                            //��������� ����� � ���������� � ������
                            self->sendServerResponse(true, false, domainName, port);

                            //�������� ������������ ������ ����� ������ � ��������
                            self->startDataTransfer();
                        }
                        else {
                            //��������� ����� � ���, ��� � ����������� � ������ �� ��������
                            self->sendServerResponse(false, false, domainName, port);
                            self->closeConnection();
                        }
                    });
            }
            else {
                self->sendServerResponse(false, false, domainName, port);
                self->closeConnection();
            }
        });
}

void Session::sendServerResponse(bool success, bool isIP,
    const std::string& destinationAddress, uint16_t port) {
    auto response = std::make_shared<std::vector<char>>();

    response->push_back(SOCKS5);
    response->push_back(success ? SUCCESS : SERVER_FAILURE); // 0x00 = ��������, 0x01 = ������
    response->push_back(RESERVE_BYTE);

    if (isIP) {
        response->push_back(IPv4); // ��� ������ - IPv4 (0x01)
        boost::asio::ip::address_v4 ip = boost::asio::ip::make_address_v4(destinationAddress);
        response->insert(response->end(), ip.to_bytes().begin(), ip.to_bytes().end());
    }
    else {
        response->push_back(DOMAIN_NAME); // ��� ������ - �������� ���
        response->push_back(char(destinationAddress.size())); // ����� �����
        response->insert(response->end(), destinationAddress.begin(), destinationAddress.end());
    }

    response->push_back(static_cast<char>(port >> 8)); // ������� ���� �����
    response->push_back(static_cast<char>(port & 0xFF)); // ������� ���� �����

    clientSocket->getSocket().async_write_some(boost::asio::buffer(*response),
        [self = shared_from_this(), response](const boost::system::error_code& err, std::size_t bytes_transferred) {
            if (err) {
                std::cerr << "�� ������� ��������� ����� " << err.message() << std::endl;
                self->closeConnection();
            }
            else {
                std::cout << "����� ������� ���������" << bytes_transferred << std::endl;
            }
        });
}

void Session::startDataTransfer() {
    auto self(shared_from_this());

    recvDataFromClient();
    recvDataFromHost();
}

void Session::sendDataToHost(size_t len) {
    auto self(shared_from_this());
    hostSocket->getSocket().async_write_some(boost::asio::buffer(hostSocket->getBuffer(), len),
        [self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                //std::cerr << "�������� ������ ����� " << length << std::endl;
                self->recvDataFromClient();
            }
            else {
                std::cerr << "������ ������: " << ec.message() << std::endl;
                self->closeConnection();
            }
        });
}

void Session::sendDataToClient(size_t len) {
    auto self(shared_from_this());
    clientSocket->getSocket().async_write_some(boost::asio::buffer(clientSocket->getBuffer(), len),
        [self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                //std::cerr << "�������� ������ ������� " << length << std::endl;
                self->recvDataFromHost();
            }
            else {
                std::cerr << "������ ������: " << ec.message() << std::endl;
                self->closeConnection();
            }
        });
}

void Session::recvDataFromHost() {
    auto self(shared_from_this());

    hostSocket->getSocket().async_read_some(boost::asio::buffer(clientSocket->getBuffer(), clientSocket->getBuffer().size()),
        [self](const boost::system::error_code& error, std::size_t length) {
            if (!error) {
                //std::cerr << "�������� ������ � ����� " << length << " ����" << std::endl;
                self->sendDataToClient(length);
            }
            else {
                std::cerr << "������ ������: " << error.message() << std::endl;
                self->closeConnection();
            }
        });
}

void Session::recvDataFromClient() {
    auto self(shared_from_this());
    clientSocket->getSocket().async_read_some(boost::asio::buffer(hostSocket->getBuffer(), hostSocket->getBuffer().size()),
        [self](const boost::system::error_code& error, std::size_t length) {
            if (!error) {
                //std::cerr << "�������� ������ � ������� " << length << " ����" << std::endl;
                self->sendDataToHost(length);
            }
            else {
                std::cerr << "������ ������: " << error.message() << std::endl;
                self->closeConnection();
            }
        });
}

void Session::closeConnection() {
    clientSocket->close();
    hostSocket->close();
}