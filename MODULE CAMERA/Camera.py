import cv2
import base64
import time
import threading
import os
import RPi.GPIO as GPIO
import socket
import paho.mqtt.client as mqtt
import signal
import sys

# Cấu hình GPIO
CAMERA_LED_PIN = 12
WIFI_LED_PIN = 24


class MQTTCamera():
    def __init__(self):
        # Khai báo thông số giao tiếp MQTT
        self.broker = "mqtt-dashboard.com"
        self.port = 1883
        self.topicCame = "optimarobotics/robot/mqtt/camera"
        self.topicVoice = "optimarobotics/robot/mqtt/voice"
        self.topicRecord = "optimarobotics/robot/mqtt/record"
        self.topicIdRobot = "optimarobotics/robot/mqtt/idrobot"
        self.topicOpenCame = "optimarobotics/robot/mqtt/opencamera"

        GPIO.cleanup()

        GPIO.setmode(GPIO.BCM)
        GPIO.setup(CAMERA_LED_PIN, GPIO.OUT)
        GPIO.setup(WIFI_LED_PIN, GPIO.OUT)

        # Cấu hình camera
        self.cam_id = 0
        self.width = 400
        self.height = 200
        self.quality = 30
        self.fps = 10
        self.correct_id_received = False
        self.client = mqtt.Client()

        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.on_publish = self.on_publish
        self.send_camera = True
        self.opencamera = False

    # Hàm đăng ký topic nhận dữ liệu
    def on_connect(self, client, userdata, flags, rc):
        client.subscribe(self.topicIdRobot)
        client.subscribe(self.topicOpenCame)

    # Xử lý dữ liệu khi nhận từ MQTT
    def on_message(self, client, userdata, msg):
        if msg.topic == self.topicOpenCame:
            if msg.payload.decode() == "opencame":
                self.opencamera = True
            else:
                self.opencamera = False
        elif msg.topic == self.topicIdRobot:
            if msg.payload.decode() == "ASR02":
                self.correct_id_received = True
            else:
                self.correct_id_received = False

    def on_publish(self, client, userdata, mid):
        print("Message Published...")

    # Kết nối MQTT
    def connect(self):
        self.client.connect(self.broker, self.port, 60)
        self.client.loop_start()

    # Ngắt kết nối MQTT
    def disconnect(self):
        self.client.loop_stop()
        self.client.disconnect()

    # Resize ảnh về kích thước setup và chuyển đổi hình ảnh sang dạng base64
    def read_resize_and_compress_image(self, image):
        resized_image = cv2.resize(image, (self.width, self.height))
        encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), self.quality]
        _, buffer = cv2.imencode('.jpg', resized_image, encode_param)
        image_data = base64.b64encode(buffer).decode('utf-8')
        return image_data

    # Publish base64 qua MQTT
    def send_image_via_mqtt(self, image):
        image_data = self.read_resize_and_compress_image(image)
        result = self.client.publish(self.topicCame, image_data)
        status = result.rc
        if status == 0:
            print(f"Image sent to topic `{self.topicCame}`")
        else:
            print(f"Failed to send image to topic `{self.topicCame}`")

    def check_internet_connection(self):
        try:
            socket.create_connection(("8.8.8.8", 53), timeout=3)
            return True
        except OSError:
            return False

    def wifi_led_blink(self, times=3, interval=0.5):
        for _ in range(times):
            GPIO.output(WIFI_LED_PIN, GPIO.HIGH)
            time.sleep(interval)
            GPIO.output(WIFI_LED_PIN, GPIO.LOW)
            time.sleep(interval)

    # Bắt đầu quá trình gửi hình ảnh từ camera
    def startcamera(self):
        self.send_camera = True
        thread = threading.Thread(target=self.run)  # Tạo một luồng mới để chạy phương thức run
        thread.start()

    # Hàm run
    def run(self):
        self.connect()

        while self.send_camera:
            if self.check_internet_connection():
                # self.wifi_led_blink(3)  # Nháy đèn 3 lần khi có kết nối Wi-Fi
                GPIO.output(WIFI_LED_PIN, GPIO.HIGH)  # Bật đèn khi có kết nối wifi
            else:
                GPIO.output(WIFI_LED_PIN, GPIO.LOW)  # Tắt  đèn khi mất kết nối Wi-Fi

            if self.opencamera:
                cap = cv2.VideoCapture(self.cam_id)
                if not cap.isOpened():
                    print("Error: Could not open video device")
                    GPIO.output(WIFI_LED_PIN, GPIO.HIGH)  # Bật đèn khi có vấn đề với camera
                    self.disconnect()
                    return

                # Bật đèn camera khi mở thành công
                GPIO.output(CAMERA_LED_PIN, GPIO.HIGH)
                wait_time = 1 / self.fps

                while self.opencamera:
                    print("KIEM TRA WIFI")
                    if not self.check_internet_connection():
                        GPIO.output(WIFI_LED_PIN, GPIO.HIGH)
                        GPIO.output(CAMERA_LED_PIN, GPIO.LOW)
                        cap.release()

                    ret, frame = cap.read()
                    if not ret or not self.send_camera:
                        print("Failed to capture image")
                        break
                    self.send_image_via_mqtt(frame)
                    time.sleep(wait_time)

                cap.release()

                # Tắt đèn khi camera dừng hoạt động
                GPIO.output(CAMERA_LED_PIN, GPIO.LOW)

            # Kiểm tra kết nối Wi-Fi sau mỗi 10 giây
            time.sleep(10)

        # Tắt đèn khi dừng hoàn toàn
        GPIO.output(CAMERA_LED_PIN, GPIO.LOW)

    # Dừng gửi hình ảnh
    def stop(self):
        self.send_camera = False
        print("STOPPPPPPPPPPPPPPPP")
        # Tắt đèn và dọn dẹp GPIO khi kết thúc chương trình
        GPIO.output(CAMERA_LED_PIN, GPIO.LOW)
        self.disconnect()
        GPIO.cleanup()

    # Tiếp tục gửi hình ảnh
    def tieptuc(self):
        print("CONTINUEEEEEEEEEEE")
        GPIO.output(CAMERA_LED_PIN, GPIO.HIGH)
        self.send_camera = True


def signal_handler(sig, frame):
    print("Program is exiting. Cleaning up GPIO...")
    GPIO.output(CAMERA_LED_PIN, GPIO.LOW)
    GPIO.output(WIFI_LED_PIN, GPIO.LOW)
    GPIO.cleanup()
    sys.exit(0)


if __name__ == "__main__":
    # Đăng ký tín hiệu ngắt
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    mqtt_camera = MQTTCamera()
    try:
        mqtt_camera.startcamera()
    except KeyboardInterrupt:
        mqtt_camera.stop()
        GPIO.cleanup()


