#include <boost/asio.hpp>
#include "speedWriter.hpp"
#include <memory>
#include <algorithm>
#include <filesystem>
#include <fstream> 
#include <iostream>

using boost::asio::ip::tcp;

std::string readUntilMinus(tcp::socket& socket);

std::string getUniqueFileName(std::string baseName);

std::vector<std::string> split(const std::string& str, char delimiter);

void createUploadsFolder();

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket);
    void start();

private:
    void readFileNameAndSize();
    void readFile();
    void doWrite(std::string massage);

    tcp::socket socket_;
    std::string fileName;
    size_t fileSize;
    std::string uploads = "uploads\\";
    static const size_t bufferSize = 8192;
    char data[bufferSize];
};