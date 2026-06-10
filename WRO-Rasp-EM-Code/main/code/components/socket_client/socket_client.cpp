#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include "socket_client.h"

using namespace std;

SocketClient::SocketClient(const string& ip, int port) {
  server_ip = ip;
  server_port = port;
  sock_fd = -1;
}

SocketClient::~SocketClient() {
  closeSocket();
}

bool SocketClient::connectToServer() {
  int socket_desc;
  struct sockaddr_in server;

  // Iipv4 + port = AF_INET
  socket_desc = socket(AF_INET, SOCK_STREAM, 1);
  if (socket_desc == 0) {
    cout << "Could nor create connect socket" << endl;
  }
  
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(8080);

  if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    puts("socket connect error");
    closeSocket();
    return 1;
  }

  return 0;
}

bool SocketClient::recvData(std::vector<unsigned char>& jpeg) {
    uint32_t net_size;
    size_t received = 0;
    char* size_ptr = reinterpret_cast<char*>(&net_size);

    while (received < sizeof(net_size)) {
        ssize_t n = recv(sock_fd, size_ptr + received,
                         sizeof(net_size) - received, 0);
        if (n <= 0) return false;
        received += n;
    }

    uint32_t frame_size = ntohl(net_size);

    jpeg.resize(frame_size);
    received = 0;
    char* data_ptr = reinterpret_cast<char*>(jpeg.data());

    while (received < frame_size) {
        ssize_t n = recv(sock_fd, data_ptr + received,
                         frame_size - received, 0);
        if (n <= 0) return false;
        received += n;
    }

    return true;
}
