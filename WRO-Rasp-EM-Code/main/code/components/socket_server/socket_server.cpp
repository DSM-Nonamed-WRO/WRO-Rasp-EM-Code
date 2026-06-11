#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include "socket_server.h"

using namespace std;
using namespace cv;

SocketServer::SocketServer(int port) {
  server_port = port;
  listen_fd = -1;
  client_fd = -1;
}

SocketServer::~SocketServer() {
  if (client_fd >= 0) close(client_fd); 
  if (listen_fd >= 0) close(listen_fd);
}

bool SocketServer::socketServerInit() {
  int socket_desc;
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) { 
    return -1;
  }

  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

bool SocketServer::socketServerWaitClient() {
  listen_fd = accept(listen_fd, nullptr, nullptr);
  return (client_fd >= 0);
}

bool SocketServer::socketServerSendFrame(const Mat& frame) {
  vector<uchar> buf;
  vector<int> params = {
    IMWRITE_JPEG_QUALITY, 
    80
  };

  if (!imencode(".jpg", frame, buf, params)) {
    return -1;
  }

  uint32_t len = htonl(static_cast<uint32_t>(buf.size()));
  if (!sendAll(reinterpret_cast<uint8_t*>(&len), sizeof(len))) {
    return -1;
  }

  if (!sendAll(buf.data(), buf.size())) {
    return -1;
  }

  return 0;
}

bool SocketServer::sendAll(const uint8_t* data, size_t len) {
  size_t sent = 0;
  while (sent < len) {
    ssize_t now = ::send(client_fd, data + sent, len - sent, 0);
    if (now <= 0) {
      return -1;
    }
    sent += now;
  }
  return 0;
}

pair<Mat, Mat> detectColor(const Mat& frame) {
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    Mat red1, red2, red_mask;
    inRange(hsv, Scalar(0,100,100),   Scalar(10,255,255),  red1);
    inRange(hsv, Scalar(160,100,100), Scalar(180,255,255), red2);
    bitwise_or(red1, red2, red_mask);
    Mat green_mask;
    inRange(hsv, Scalar(40,100,100), Scalar(80,255,255), green_mask);
    return {red_mask, green_mask};
}

string decideDirection(const Mat& frame, const Mat& red_mask, const Mat& green_mask) {
    int cx = frame.cols / 2;
    vector<pair<Mat,string>> targets = {{red_mask, "RIGHT"}, {green_mask, "LEFT"}};
    for (auto& [mask, dir] : targets) {
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if (contours.empty()) continue;
        auto& c = *max_element(contours.begin(), contours.end(),
            [](const vector<Point>& a, const vector<Point>& b){
                return contourArea(a) < contourArea(b);
            });
        if (contourArea(c) < 500) continue;
        Rect bbox = boundingRect(c);
        if (bbox.x < cx && cx < bbox.x + bbox.width)
            return dir;
    }
    return "FORWARD";
}

void SocketServer::run() {
  string pipeline =
    "v4l2src device=/dev/video0 ! video/x-raw,width=640,height=480,"
    "framerate=30/1 ! videoconvert ! appsink";
  
  VideoCapture cap(pipeline, CAP_GSTREAMER);
  if (!cap.isOpened()) {
    cerr << "Couldn't open camera" << endl;
  }

  Mat frame;
    while (true) {
      cap >> frame;
      if (frame.empty()) {
        continue;
      }

      auto [red_mask, green_mask] = detectColor(frame);
      string direction = decideDirection(frame, red_mask, green_mask);

      putText(frame, direction, Point(10, 50),
          FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 255, 0), 3);

      if (!socketServerSendFrame(frame)) {
          cerr << "failed to send frame, close the client connect" << endl;
          break;
        }
    }
}
