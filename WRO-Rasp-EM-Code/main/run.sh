#!/bin/bash
sudo killall -9 wro python3 rpicam-vid rpicam-still libcamera-vid 2>/dev/null
sudo systemctl restart nvgettext 2>/dev/null
rm -f /tmp/frame.jpg
sleep 0.5

cd /home/wro/wro_project/WRO-Rasp-EM-Code/main/code

echo "C++ 컴파일 중..."
g++ -O2 -o wro main.cpp $(pkg-config --cflags --libs opencv4)

echo "C++ 프로그램 실행..."
./wro &
libcamerify ./wro &

sleep 1.0

echo "파이썬 Flask 스트리밍 서버 시작..."
python3 stream.py
