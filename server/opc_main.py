from camera import Camera
from opc_server import OpcServer
from move_opc import Move
from My_relay import Conveyor
from time import sleep

cam = 0
camera = Camera(cam, "work")

opc_server = OpcServer("opc.tcp://0.0.0.0:4840")
mv = Move(camera, opc_server)
camera.configure()
camera.static_calibration_2()
print(camera.H_matrix)

Yep = True

agr = False
start = input("START MOVEMENT? (y/n)")
mv.start_video(Yep)
# while not agr:
#     mv.speed_define(duration=700, perspective=Yep)
#     sleep(1)
#     mv.rotattion_define(perspective=Yep)

#     print(f"rotation speed {mv.speed}")
#     print(f"rotarion {mv.rotation}")
#     q = input("REDEFINE MOVE CONSTANS? (y/n)")
#     agr = (q != "y")

mv.speed = 0.10200016196949765
mv.rotation = 0.3323253483301088

start = input("START MOVEMENT? (y/n)")
while start != "n":
    mv.get_target_point("green", Yep)
    print(mv.camera.color_location['green'])
    # while not mv.reached(target='green'):
    mv.commander(['conveyor', 'green'], perspective=Yep)
    print("wait for conveyor")
    # conveyor = Conveyor()
    # conveyor.execute(2)
    sleep(2)
    print("Conveyor executed")
    mv.get_target_point("yellow", Yep)
    print(mv.camera.color_location['yellow'])
    # while not mv.reached(target="yellow"):
    mv.commander(perspective=Yep)
    start = input("START MOVEMENT? (y/n)")
