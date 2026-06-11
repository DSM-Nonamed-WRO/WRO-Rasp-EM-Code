#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <iostream>

using namespace std;
using namespace cv;

class SocketServer {
private:
  bool sendAll(const uint8_t* data, size_t len);

  int server_port;
  int listen_fd;
  int client_fd;

public:
  SocketServer(int port);
  ~SocketServer();

  bool socketServerInit();
  bool socketServerWaitClient();
  bool socketServerSendFrame(const Mat& frame);
  void run();
}
#endif
