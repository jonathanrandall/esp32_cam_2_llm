import http.server
import socketserver

from pydub import AudioSegment
from gtts import gTTS
import os
import pyttsx3
import io



import requests
import cv2
import threading
import queue
import ollama
import time
from PIL import Image

run_inference = False

start_time = time.time()

def get_b_array(image_path):
    with open(image_path, 'rb') as image_file:
        img1 = Image.open(image_file)
        
        # Convert the image to a byte stream in JPEG format
        img_byte_arr = io.BytesIO()
        img1.save(img_byte_arr, format='JPEG')  # or 'PNG' if you prefer
        img_byte_arr = img_byte_arr.getvalue()
    return img_byte_arr

class VideoCapture:

    def __init__(self, name):
        self.cap = cv2.VideoCapture(name)#, apiPreference=cv2.CAP_V4L2)
        # self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, width*mult)
        # self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height*mult)
        self.q = queue.Queue()
        t = threading.Thread(target=self._reader)
        t.daemon = True
        t.start()

    # read frames as soon as they are available, keeping only most recent one
    def _reader(self):
        while True:
            ret, frame = self.cap.read()
            if not ret:
                break
            if not self.q.empty():
                try:
                    self.q.get_nowait()   # discard previous (unprocessed) frame
                except queue.Empty:
                    pass
            self.q.put(frame)

    def read(self):
        return True, self.q.get()
    

esp32_ip = "http://192.168.1.130"
cap = cv2.VideoCapture(esp32_ip + ":81/stream")


def text_to_voice(text, fout = "output.mp3"):
    tts = gTTS(text=text, lang='en')
    tts.save(fout)

def text_to_voice_local(text, fout="output.mp3"):
    engine = pyttsx3.init()
    engine.setProperty('rate', 150)    # Speed of speech
    engine.setProperty('volume', 1.0)  # Volume level (0.0 to 1.0)
    # engine.setProperty('voice', voices[1].id) 
    engine.setProperty('voice', 'english+f1')
    # engine.setProperty('female', voice.gender)

    engine.save_to_file(text, 'output.wav')  
    engine.runAndWait()
    input_wav = "output.wav"
    audio = AudioSegment.from_wav(input_wav)

    audio.export(fout, format="mp3", bitrate="96k", parameters=["-ac", "1"])



def stream_frames():
    global esp32_ip
    global run_inference
    img_path = 'img.jpg'
    already_done =False
    while True:
        
        if cap.isOpened():
            res, img = cap.read()
            if res:
                cv2.imshow("left", img)
                
                key1 = cv2.waitKey(2)
                # if not already_done and (time.time() - start_time > 5):
                #     run_inference = True
                #     already_done = True
                
                cv2.imwrite(img_path, img)
                
                if key1 == ord('p'):
                    run_inference = True
                    break
                if key1 == ord('q'):
                    break

                
            
            
       
        if run_inference:
            print(run_inference)
            
            run_inference = False
            
            
            image_bytearray = get_b_array(img_path) #bytearray(buffer)
            print('responding')
            response = ollama.chat(model='llava', messages=[
            {
                'role': 'user',
                "prompt":"What is in this picture?",
                "images": [image_bytearray]
                },
            ])
            print('responded')
            text = (response['message'])['content']
            text_to_voice(text)
            #tell esp32 we are ready
            resp = requests.get(esp32_ip+f"/job")
            run_inference = False


print('here')

# Define the handler to serve the file
class MyHttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        global run_inference
        if self.path == '/output.mp3':
            self.path = 'output.mp3'  # Your MP3 file path
        elif self.path == '/request_inference':
            run_inference = True
            self.send_response(200)
            self.send_header("Content-type", "text/plain")
            self.end_headers()
            self.wfile.write(b"")  # Return empty body (NULL equivalent)
            return  # End the request here
        return http.server.SimpleHTTPRequestHandler.do_GET(self)

# Create the server
PORT = 8000
socketserver.TCPServer.allow_reuse_address = True

# handler = MyHttpRequestHandler
# httpd = socketserver.TCPServer(("", PORT), handler)

# print(f"Serving at port {PORT}")
# httpd.serve_forever()

# Start the streaming thread
stream_thread = threading.Thread(target=stream_frames)
stream_thread.daemon = True
stream_thread.start()

# Start the HTTP server
with socketserver.TCPServer(("", PORT), MyHttpRequestHandler) as httpd:
    print(f"Serving HTTP stream on port {PORT}")
    httpd.serve_forever()

# Keep the script running
try:
    while True:
        pass
except KeyboardInterrupt:
    print("Stopping server...")
    httpd.shutdown()

