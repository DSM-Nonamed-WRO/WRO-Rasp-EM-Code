#!/bin/bash
sudo lsof -i :8080
sudo killall -9 wro python3 rpicam-vid rpicam-still 2>/dev/null
sudo systemctl restart nvgettext 2>/dev/null
rm -f /tmp/frame.jpg
sleep 0.5

cd /home/wro/wro_project/WRO-Rasp-EM-Code/main/code

echo "C++ 빌드 중..."
cmake -B build
cmake --build build

echo "C++ 서버 실행..."
./build/wro

#./wro &
#libcamerify ./wro &

#sleep 1.0

#echo "파이썬 Flask 스트리밍 서버 시작..."
#python3 stream.py



# Flask -> local pc 
# Server -> Raspi Server
#
# Flask를 local에서 돌릴 때 ip가 local ip여야하므로
# 따로 python3 stream.py 입력
#
echo "카메라가 열리지 않는다면"
echo "libcamerify ./build/wro 실행"
echo "python3 stream.py 실행"
