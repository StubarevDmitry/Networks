#include <iostream>
#include "server.hpp"
#include "client.hpp"
#include <thread>


int main() {
    int choice;

    setlocale(LC_ALL, "Russian");
    std::cout << "Выберите режим работы:\n1. Сервер\n2. Клиент\nВаш выбор: ";
    std::cin >> choice;
    std::cin.ignore(); // Игнорируем оставшийся символ новой строки

    if (choice == 1) {
        try {
            boost::asio::io_context io_context;
            Server server(io_context, 12345);
            io_context.run();
        }
        catch (std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
    else if (choice == 2) {
        try {
            std::string ip;
            std::string port;
            std::cout << "IP сервера для коннекта:";
            std::cin >> ip;
            std::cout << "Port сервера для коннекта:";
            std::cin >> port;

            Client client(ip, port);
            client.start();
        }
        catch (std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
    else {
        std::cerr << "Неверный выбор." << std::endl;
    }

    return 0;
}