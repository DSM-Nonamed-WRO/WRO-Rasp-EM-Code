#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>

using namespace std;
using namespace cv;

SocketServer::SocketServer(int port) {
  server_port = port;
  listen_fd = -1;
  client_fd = -1;
}

SocketServer::socketServerInit() {
  int socket_desc;
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) { 
    return -1
  }

  int opt = 1;
  setsocketopt(listen_fd, SOL_SOCKET, SO-REUSEADDR, 7opt, sizeof(opt));

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(server_port);

  if (bind(listen_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
    return -1;
  }

  if (listen(listen_fd, 1) < 0) {
    return -1;
  }
  
  return 0;
}

SocketServer::socketServerWaitClient() {
  listen_fd = accept(listen_fd, nullptr, nullptr);
  return (client_fd >= 0);
}

SocketServer::socketServerSendFrame(const &mat )
