#include "client.hpp"


std::string getFileName(const std::string& fullPath) {
    size_t pos = fullPath.find_last_of("\\/");

    if (pos != std::string::npos) {
        return fullPath.substr(pos + 1);
    }

    return fullPath;
}

Client::Client(const std::string& host, const std::string& service)
    : socket_(io_context_) {
    boost::asio::ip::tcp::resolver resolver(io_context_);
    boost::asio::connect(socket_, resolver.resolve(host, service));
}

void Client::start() {
    std::string message;
    std::cout << "������� ��� ����� (��� 'exit' ��� ������): ";
    std::cin >> message;
    writeFile(message);
}

void Client::writeFile(const std::string filename) {

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "�� ������� ������� ����." << std::endl;
        return ;
    }

    // ���������� ������ �����
    file.seekg(0, std::ios::end);
    std::size_t filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    // ���������� ��� ����� � ������ �����
    boost::asio::write(socket_, boost::asio::buffer(std::to_string(filesize) + " " + getFileName(filename) + "-"));

    // ���������� ���������� �����
    char buffer[bufferSize];

    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        std::streamsize bytes_read = file.gcount();
        boost::asio::write(socket_, boost::asio::buffer(buffer, bytes_read));
    }

    if (check()) {
        std::cout << "������ � ���������" << std::endl;
    }

    file.close();
}

bool Client::check() {
    size_t length = socket_.read_some(boost::asio::buffer(data_));
    if (strcmp(data_,"ERROR") == 0) {
        return false;
    }
    else {
        return true;
    }
}