from picamera.array import PiRGBArray
from picamera import PiCamera
import cv2
import RPi.GPIO as GPIO
import os
import shutil

GPIO.setmode(GPIO.BCM)
GPIO.setup(20, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# Xóa dữ liệu cũ và tạo thư mục mới
data_path = 'dataSet'
if os.path.exists(data_path):
    shutil.rmtree(data_path)
os.makedirs(data_path)

# Khởi tạo camera
camera = PiCamera()
camera.resolution = (480, 280)
camera.framerate = 24
rawCapture = PiRGBArray(camera, size=(480, 280))

# Load file cascade cho nhận diện khuôn mặt
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
print("\n Please enter password from key matrix !!")

while True:
    if GPIO.input(20) == False:
        face_id = input("\n Enter user id :") 
        print("\n [INFO] Initializing face capture. Look the camera and wait ...")
        count = 0
        for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
            image = frame.array
            image = cv2.flip(image, 1)
            gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
            faces = face_cascade.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=5, minSize=(100, 100), flags=cv2.CASCADE_SCALE_IMAGE)

            for (x, y, w, h) in faces:
                roi_gray = gray[y:y+h, x:x+w]
                cv2.rectangle(image, (x, y), (x+w, y+h), (255, 0, 0), 2)
            
            if len(faces):
                count += 1
                img_item = f"dataSet/User.{face_id}.{count}.jpg"
                cv2.imwrite(img_item, roi_gray)

            img2 = cv2.resize(image, (960, 560))
            cv2.imshow("Frame", img2)

            if cv2.waitKey(1) & 0xff == ord("q"):
                exit()
            if count == 300:
                print("\n _____________ SUCCESSFUL !!______________")
                exit()
            rawCapture.truncate(0)
