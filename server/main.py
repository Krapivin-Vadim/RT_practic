from camera import Camera
from telegram import Drink_offer_bot
from telegram import TOKEN
from mosquitto import Mosquitto
import threading
from move import Move
from time import sleep

cam = "https://192.168.1.65:8080/video"
mqtt_pub_topic = "test/topic"
mqtt_sub_topic = "test/topic"

if __name__ == "__main__":
    camera = Camera(cam)
    telegram_bot = Drink_offer_bot(TOKEN)
    mosquitto = Mosquitto(mqtt_pub_topic, mqtt_sub_topic)
    mv = Move(camera, mosquitto)

    agr = False

    if not agr:
        mv.calibration()
        print(f"rotation speed {mv.speed}")
        print(f"rotarion {mv.rotation}")
        q = input("REDEFINE MOVE CONSTANS? (y/n)")
        agr = (q == "y")

    telegram_thread = threading.Thread(target=telegram_bot.run_bot)
    telegram_thread.start()

    while telegram_bot.running:
        if len(telegram_bot.orders) == 0:
            sleep(2)
            continue
        telegram_bot.pop_order()
        mv.commander()

    mosquitto.pub("stop 0")
    print("Stoping")
    telegram_thread.join()
