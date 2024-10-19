#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio.hpp>
#include<string.h>
#include <fstream> 
#include <iostream>

class Client {
public:
    Client(const std::string& host, const std::string& service);
    void start();

private:
    void writeFile(const std::string file);
    bool check();
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::socket socket_;
    static const size_t bufferSize = 8192;
    char data_[bufferSize];
};

#endif // CLIENT_HPP