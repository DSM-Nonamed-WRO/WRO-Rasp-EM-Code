#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

pair<Mat,Mat> detectColor(const Mat& frame) {
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

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "카메라 열기 실패" << endl;
        return -1;
    }
    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) continue;
        auto [red_mask, green_mask] = detectColor(frame);
        string direction = decideDirection(frame, red_mask, green_mask);
        cout << direction << endl;
        putText(frame, direction, Point(10,50),
                FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0,255,0), 3);
        imwrite("/tmp/frame.jpg", frame);
    }
    return 0;
}
