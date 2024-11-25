#include "socks5Server.hpp"
#include "windows.h"  
#include "winsock.h"  
#include <iostream>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/host_name.hpp>

using namespace Server;

int main()
{
    setlocale(LC_ALL, "Russian");
    boost::asio::io_context io_context;
    Socks5Server server(io_context, 12345);
    io_context.run();
}
