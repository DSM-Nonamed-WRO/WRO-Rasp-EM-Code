#!/bin/bash
# 1. 기존 카메라 찌꺼기 다 청소
sudo killall -9 wro python3

# 2. C++ 프로그램 빌드
g++ -O2 -o wro main.cpp $(pkg-config --cflags --libs opencv4)

# 3. [핵심] C++ 프로그램을 백그라운드(&)로 실행 (바로 파이썬으로 넘어감)
./wro &

# 4. 잠시 대기 후 Flask 서버 실행
time.sleep(2)
python3 stream.py
