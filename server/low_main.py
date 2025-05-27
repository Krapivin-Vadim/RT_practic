from camera import Camera
from mosquitto import Mosquitto
from move import Move

cam = 0
# cam = "http://192.168.1.101:8080/video"
camera = Camera(cam, "work")

mqtt = Mosquitto("test/topic", "test/topic")

mv = Move(camera, mqtt)

teleg = Mosquitto("telegram/topic", "telegram/topic")

camera.configure()

agr = False

if not agr:
    mv.speed_define()
    mv.rotattion_define()
    print(f"rotation speed {mv.speed}")
    print(f"rotarion {mv.rotation}")
    q = input("REDEFINE MOVE CONSTANS? (y/n)")
    agr = (q == "y")

teleg.get_messsage()
while teleg.buffer != "stop":
    while mv.reached():
        mv.commander()
    teleg.get_messsage()
mqtt.pub("stop 0")
