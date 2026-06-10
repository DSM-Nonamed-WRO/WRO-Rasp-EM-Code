#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <string>
using namespace std;

class SocketClient {
private:
    int sock_fd;
    string server_ip;
    int server_port;

public:
    SocketClient(const string& ip, int port);
    ~SocketClient();

    bool connectToServer();
    // only use recv
    bool sendData(const void* data, size_t size);
    void closeSocket();
};

#endif:
