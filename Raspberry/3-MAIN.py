from picamera.array import PiRGBArray
from picamera import PiCamera
import cv2
import RPi.GPIO as GPIO
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders

fromaddr = "tuannguyenquy0307@gmail.com"
toaddr = "tuannguyenquy0307@gmail.com"
server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login(fromaddr, "sxbsnnhctbwrriqi")

GPIO.setmode(GPIO.BCM)
GPIO.setup(16, GPIO.OUT)
GPIO.setup(21, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.output(16, 1)

camera = PiCamera()
camera.resolution = (480, 280)
camera.framerate = 24
rawCapture = PiRGBArray(camera, size=(480, 280))

recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read('trainer/trainer.yml')
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
font = cv2.FONT_HERSHEY_SIMPLEX
names = ['ID0', 'ID1', 'ID2', 'DucNguyen']

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    image = frame.array
    image = cv2.flip(image, 1)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=5, minSize=(100, 100))

    for (x, y, w, h) in faces:
        roi_gray = gray[y:y+h, x:x+w]
        cv2.rectangle(image, (x, y), (x+w, y+h), (255, 0, 0), 2)
        id, confidence = recognizer.predict(gray[y:y+h, x:x+w])
        
        if confidence < 70:
            id = names[id]
            confidence_text = f"  {100 - confidence}%"
            GPIO.output(16, 0)
            cv2.putText(image, str(id), (x+5, y-5), font, 1, (255, 255, 255), 2)
            cv2.putText(image, confidence_text, (x+5, y+h-5), font, 1, (255, 255, 0), 1)
        else:
            id = "KHONG NHAN DIEN DUOC"
            id2 = "VUI LONG NHAP MAT KHAU"
            confidence_text = f"  {100 - confidence}%"
            GPIO.output(16, 1)
            cv2.putText(image, str(id), (x-40, y-20), font, 0.5, (0, 0, 255), 1)
            cv2.putText(image, str(id2), (x-40, y-5), font, 0.5, (0, 0, 255), 1)

    img2 = cv2.resize(image, (960, 560))
    cv2.imshow("Frame", img2)

    if GPIO.input(21) == False:
        cv2.imwrite("DOT_NHAP.jpg", img2)
        body = "NHAP SAI MAT KHAU QUA 3 LAN CHO PHEP!"
        try:
            msg = MIMEMultipart()
            msg['From'] = fromaddr
            msg['To'] = toaddr
            msg['Subject'] = "THÔNG BÁO!!!"
            msg.attach(MIMEText(body, 'plain'))
            filename = "DOT_NHAP.jpg"
            with open(filename, "rb") as attachment:
                part = MIMEBase('application', 'octet-stream')
                part.set_payload(attachment.read())
                encoders.encode_base64(part)
                part.add_header('Content-Disposition', f"attachment; filename= {filename}")
                msg.attach(part)
                server.sendmail(fromaddr, toaddr, msg.as_string())
                print("ĐÃ GỬI MAIL")
        except Exception as e:
            print("Lỗi khi gửi email:", e)
        
    if cv2.waitKey(1) & 0xff == ord("q"):
        break
    rawCapture.truncate(0)

server.quit()
