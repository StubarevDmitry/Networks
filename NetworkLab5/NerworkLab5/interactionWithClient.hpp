#ifndef Interaction_with_client
#define Interaction_with_client

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <array>
#include <iostream>
#include <functional>

using boost::asio::ip::tcp;

#define BufferSize 32384

class interactionWithClient
{
public:
    using ReadHandler = std::function<void(const boost::system::error_code&, std::size_t)>;
    using WriteHandler = std::function<void(const boost::system::error_code&, std::size_t)>;

    interactionWithClient(tcp::socket socket);

    ~interactionWithClient();

    interactionWithClient(interactionWithClient&& other) noexcept;
    interactionWithClient& operator=(interactionWithClient&& other) noexcept;

    interactionWithClient(const interactionWithClient&) = delete;
    interactionWithClient& operator=(const interactionWithClient&) = delete;

    tcp::socket& getSocket();
    std::array<char, BufferSize>& getBuffer();

    void asyncRead(ReadHandler handler);
    void asyncWrite(const std::string& message, WriteHandler handler);

private:
    tcp::socket socket_;
    std::array<char, 32384> clientBuffer;
};

#endif