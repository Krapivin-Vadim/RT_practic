from camera import Camera
from telegram import Drink_offer_bot
from telegram import TOKEN
from mosquitto import Mosquitto
import threading
from numpy.linalg import norm

cam = 0
mqtt_pub_topic = "pub/topic"
mqtt_sub_topic = "sub/topic"

if __name__ == "__main__":
    camera = Camera(cam)
    camera.imget()
    # camera.calibration('blue')

    telegram_bot = Drink_offer_bot(TOKEN)

    mosquitto = Mosquitto(mqtt_pub_topic, mqtt_sub_topic)

    telegram_thread = threading.Thread(target=telegram_bot.run_bot)
    telegram_thread.start()

    while telegram_bot.running:
        if len(telegram_bot.orders) == 0:
            continue
        current_order = telegram_bot.pop_order()
        camera.imget()
        camera.to_hsv()
        camera.build_masks()
        for color in camera.color_location.keys():
            camera.get_color_location(color)
        # camera.fix_location()
        camera.get_middle_location()
        camera.get_vector('blue', 'red')
        camera.get_vector('middle', 'green')
        camera.get_angle()
        try:
            module = norm(camera.color_vector['middle-green'])
        except Exception:
            module = 0.0
        mosquitto.pub(f"Turn {camera.angle}")
        mosquitto.pub(f"forward {module}")

        # mosquitto.get_messsage()
        # print(mosquitto.buffer)

    print("Stoping")
    telegram_thread.join()
