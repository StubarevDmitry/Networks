namespace Server::ServerConst {
    enum ProxyConst
    {
        SOCKS5 = 0x05,
        NO_AUTH = 0x00,
        NO_ACCEPTABLE_METHODS = 0xff,
        TCP_CONNECT = 0x01,
        RESERVE_BYTE = 0x00,
        IPv4 = 0x01,
        DOMAIN_NAME = 0x03,
        SUCCESS = 0x00,
        SERVER_FAILURE = 0x01
    };
}