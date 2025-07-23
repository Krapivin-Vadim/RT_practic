from camera import Camera
from opc_server import OpcServer
from move_opc import Move
from My_relay import Conveyor
from time import sleep
from os.path import abspath

PATH = '\\'.join([i for i in abspath(__file__).split("\\")[:-1]])

cam = 0
camera = Camera(cam, "work")

opc_server = OpcServer("opc.tcp://0.0.0.0:4840")
mv = Move(camera, opc_server)
camera.configure()
# camera.static_calibration_2()
# print(camera.H_matrix)

Yep = False

agr = False
mv.start_video(Yep)
start = input("START MOVEMENT? (y/n)")
while not agr:
    mv.speed_define(duration=700, perspective=Yep)
    sleep(1)
    input("Press any button to continue colibration")
    mv.rotattion_define(duration=300, perspective=Yep)

    print(f"rotation speed {mv.speed}")
    print(f"rotarion {mv.rotation}")
    q = input("REDEFINE MOVE CONSTANS? (y/n)")
    agr = (q != "y")

with open(rf"{PATH}\move_constants.conf", "w") as config:
    config.write(f"{mv.speed}\n")
    config.write(f"{mv.rotation}")
