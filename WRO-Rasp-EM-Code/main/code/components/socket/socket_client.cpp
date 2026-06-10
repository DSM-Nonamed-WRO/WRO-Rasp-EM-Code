#include <iostream.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket_client.h"

using namespace std;

SocketClient::SocketClient(const string& ip, int port) {
  server_ip = ip;
  server_port = port;
  sock_df = -1;
}

SocketClient::~SocketClient() {
  closeSocket();
}

bool SocketClient::connectToServer() {
  int socket_desc;
  struct sockaddr_in server;

  // Iipv4 + port = AF_INET
  socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_desc == 0) {
    printf("Could nor create connect socket\n");
  }
  
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(8080);

  if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    puts("socket connect error");
    closerSocket();
    return 1;
}

/*
bool SocketClient::sendData(const void *data, size_t size) {
  
}
*/
