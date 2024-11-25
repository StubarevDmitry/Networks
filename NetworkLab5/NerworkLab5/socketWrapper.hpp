#ifndef SocketWrapper_hpp
#define SocketWrapper_hpp

#include <boost/asio.hpp>
#include <array>
#include <memory>

#define BUFFER_SIZE 8096 * 4

using boost::asio::ip::tcp;

namespace Server {
    class SocketWrapper : public std::enable_shared_from_this<SocketWrapper> {
    public:
        SocketWrapper(boost::asio::io_context& io_context)
            : socket(io_context) {}

        tcp::socket& getSocket() { return socket; }
        std::array<char, BUFFER_SIZE>& getBuffer() { return buffer; }

        void close() { socket.close(); }

    private:

        tcp::socket socket;
        std::array<char, BUFFER_SIZE> buffer;
    };
}

#endif