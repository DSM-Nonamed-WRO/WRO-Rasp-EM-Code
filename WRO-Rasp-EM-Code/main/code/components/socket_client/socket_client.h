#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <string>
#include <vector>

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
    bool recvData(std::vector<unsigned char>& jpeg);
    void closeSocket();
};

#endif
