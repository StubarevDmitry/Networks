#include "server.hpp"
#include "speedtest.hpp"



Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

bool fileExists(std::string filename) {
    std::ifstream file(filename);
    return file.good();
}

std::string read_until_minus(tcp::socket& socket) {
    std::string received_data;
    char c;

    while (true) {
        // ��������� ���� ���� �� ������
        boost::system::error_code error;
        size_t bytes_read = socket.read_some(boost::asio::buffer(&c, 1), error);

        if (error) {
            std::cerr << "Error reading from socket: " << error.message() << std::endl;
            return " "; // ���������� " "
        }

        // ��������� ��������� ������ � ������
        received_data += c;

        if (c == '-') {
            break;
        }
    }

    return received_data;
}

std::string getUniqueFileName(std::string baseName) {
    std::string newName = baseName;
    int copyNumber = 1;

    // ���������, ���������� �� ����, ���� �� - ���������� ����� ���
    while (std::filesystem::exists("uploads\\" + newName)) {
        
        // ��������� ��� ����� � ��� ����������
        std::string::size_type dotPos = baseName.find_last_of('.');
        std::string nameWithoutExt = (dotPos == std::string::npos) ? baseName : baseName.substr(0, dotPos);
        std::string extension = (dotPos == std::string::npos) ? "" : baseName.substr(dotPos);

        // ���������� ��� ������ �����
        newName = nameWithoutExt + "(" + std::to_string(copyNumber) + ")" + extension;
        copyNumber++;
    }

    return newName;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens; // ������ ��� ���������� ����

    std::string token; // ���������� ��� �������� �������� �����

    std::istringstream tokenStream(str); // ������������� stringstream ��� ��������� ������

    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) { 
            tokens.push_back(token);
        }
    }

    return tokens;
}

void Session::start() {
    readFileNameAndSize();
}


void Session::readFileNameAndSize() {

    std::string file_metadata = read_until_minus(socket_);

    file_metadata.pop_back();

    std::vector<std::string> words = split(file_metadata, ' ');

    std::cout << "�������� ��� �����: " << words[1] << std::endl;
    fileName = words[1];

    fileSize = std::atoi(words[0].data());
    std::cout << "�������� ������ �����: " << fileSize << std::endl;
    
    readFile();
}

void Session::readFile() {
    auto self(shared_from_this());

    std::ofstream ofs(uploads + getUniqueFileName(fileName), std::ios::binary);

    SpeedTest test;
    test.timerRun();
    std::size_t total_read = 0;

    while (total_read != fileSize) {
        std::size_t lengthToRead = socket_.read_some(boost::asio::buffer(data, size_t(bufferSize)));

        //������
        ofs.write(data, lengthToRead);

        total_read += lengthToRead;

        test.addBytes(lengthToRead);

        test.timeCheckAndUpdateTime();
    }

    test.writeSpeedAndUpdateTime();

    if (!ofs) {
        std::cout << "�� ������ � ���������� " << std::endl;
        doWrite("ERROR");
    }
    else {
        std::cout << "������ � ���������� " << std::endl;
        doWrite("all good");
    }

    ofs.close();
    socket_.close();
}


Server::Server(boost::asio::io_context& io_context, short port)
    : acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
    //std::cout <<" IP: " << acceptor.local_endpoint().address() <<" Port:" << port <<"\n";
    doAccept();
}

void Server::doAccept() {
    acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                // ������� ����� ������
                auto session = std::make_shared<Session>(std::move(socket));

                // ������ ����� ������
                std::thread([session]() {
                    session->start();
                }).detach();
            }

            // ������� ��� ���� �����
            doAccept();
        });
}

void Session::doWrite(std::string massage) {
    boost::asio::write(socket_, boost::asio::buffer(massage));
}