#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <fstream> 
#include <iostream>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket);
    void start();

private:
    void readFileNameAndSize();
    void readFileSize();
    void readFile();
    void doWrite(std::string massage);

    tcp::socket socket_;
    std::string fileName;
    size_t fileSize;
    std::string uploads = "uploads\\";
    static const size_t bufferSize = 8192;
    char data[bufferSize];
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port);

private:
    void doAccept();
    tcp::acceptor acceptor;
};

