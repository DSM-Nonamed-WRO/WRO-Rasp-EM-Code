#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <string>

class SocketClient {
private:
    int sock_fd;
    std::string server_ip;
    int server_port;

public:
    SocketClient(const std::string& ip, int port);
    ~SocketClient();

    bool connectToServer();
    bool sendData(const void* data, size_t size);
    void closeSocket();
};

#endif // SOCKET_CLIE_H
