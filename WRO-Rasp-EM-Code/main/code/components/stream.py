import socket
import struct
import cv2
import numpy as np
from flask import Flask, Response

SERVER_IP = "192.168.0.xx"   
SERVER_PORT = 8080

app = Flask(__name__)


def recv_all(sock, n):
    """정확히 n바이트를 다 받을 때까지 반복해서 읽는다."""
    buf = b""
    while len(buf) < n:
        chunk = sock.recv(n - len(buf))
        if not chunk:         
            return None
        buf += chunk
    return buf


def frame_generator():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((SERVER_IP, SERVER_PORT))

    try:
        while True:
            # htonl
            header = recv_all(sock, 4)
            if header is None:
                break
            frame_len = struct.unpack('>I', header)[0]

            data = recv_all(sock, frame_len)
            if data is None:
                break

            img = cv2.imdecode(np.frombuffer(data, np.uint8), cv2.IMREAD_COLOR)
            if img is None:
                continue

            ok, jpg = cv2.imencode('.jpg', img)
            if not ok:
                continue

            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + jpg.tobytes() + b'\r\n')
    finally:
        sock.close()


@app.route('/video_feed')
def video_feed():
    return Response(frame_generator(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')


@app.route('/')
def index():
    return '<img src="/video_feed">'


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, threaded=True)
