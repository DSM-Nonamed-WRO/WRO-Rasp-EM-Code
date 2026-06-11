#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

#include "socket_server.h"

using namespace cv;
using namespace std;

int main() {
    SocketServer server(8080);
    if (!server.socketServerInit()) {
      { std::cerr << "init 실패\n"; return -1; }
      return -1;
    }

    cout << "Flask 연결 대기 중...\n";
    if (!server.socketServerWaitClient()) { 
      cerr << "accept 실패\n"; return -1;
      return -1;
    }

    cout << "연결됨, 스트리밍 시작\n";
    server.run();
    return 0;
}
