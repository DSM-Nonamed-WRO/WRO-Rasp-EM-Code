import subprocess
import time
from flask import Flask, render_template, Response

app = Flask(__name__)

def GenerateFrames():
    cmd = [
        'rpicam-vid',
        '-t', '0', 
        '--inline', 
        '--width', '1024', 
        '--height', '768', 
        '--framerate', '25', 
        '--codec', 'mjpeg',
        '--flush',
        '-o', '-'
    ]
    
    # stdout=subprocess.PIPE로 바이너리를 직접 받아옵니다.
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    
    try:
        while True:
            # MJPEG의 프레임 시작점(0xFF 0xD8)을 찾기 위해 헤더를 파싱합니다.
            header = process.stdout.read(4)
            if not header:
                break
                
            # 데이터 스트림이 정상적이지 않을 경우 한 바이트씩 건너뛰며 동기화
            if header[0:2] != b'\xff\xd8':
                continue
                
            # 이미지 바이트 데이터를 묶어서 전송하는 로직
            # --inline 옵션 덕분에 패킷이 깨지지 않고 들어옵니다.
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + header + process.stdout.read(65536) + b'\r\n')
    except Exception:
        pass
    finally:
        process.terminate()

@app.route('/')
def Index():
    return render_template('index.html')

@app.route('/stream')
def Stream():
    return Response(GenerateFrames(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, threaded=True)
