#include "boost/asio.hpp"
#include "receiver.hpp"
#include "TemporaryStorage.hpp"
#include "sender.hpp"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: receiver <listen_address> <multicast_address>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 239.255.0.1\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 ff31::8000:1234\n";
            return 1;
        }
        TemporaryStorage* ts = new TemporaryStorage();
        boost::asio::io_context io_context;
        Receiver r(io_context,
            boost::asio::ip::make_address(argv[1]),
            boost::asio::ip::make_address(argv[2]), ts);
        Sender s(io_context, boost::asio::ip::make_address(argv[2]));
        std::thread th1(&TemporaryStorage::run, ts);
        io_context.run();

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}