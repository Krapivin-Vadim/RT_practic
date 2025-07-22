from camera import Camera
from opc_server import OpcServer
from time import sleep
import numpy as np
import cv2
import threading


class Move:
    def __init__(self, camera: Camera, brokker: OpcServer):
        self.camera = camera
        self.brokker = brokker
        self.brokker.start()
        self.stop_show_event = threading.Event()
        self.Video = threading.Thread(target=self.life_time_show)
        self.suka_points = None

    def __del__(self):
        self.stop_show_event.set()
        if self.Video.is_alive():
            threading.Thread.join(self.Video)

    def start_video(self, perspective=False):
        self.perspective = perspective
        self.Video.start()

    def life_time_show(self):
        while not self.stop_show_event.is_set():
            self.get_params(self.perspective)
            self.camera.show(1, 'current')
            frame = self.camera.frame
            # self.suka()
            if self.suka_points is None:
                continue
            for item in self.suka_points:
                if item is None:
                    return
                cv2.circle(frame, [int(item[0]), int(item[1])],
                           5, (0, 255, 0), 5)
            cv2.imshow("suka", frame)
            cv2.waitKey(1)

        return

    def setup(self):
        self.camera.imget()
        self.camera.to_hsv()
        self.camera.build_masks()

    def get_params(self, perspective=False):
        self.setup()
        for color in self.camera.color_location.keys():
            self.camera.get_color_location(color)
        if perspective:
            self.camera.fix_location()
        self.camera.get_middle_location()
        self.camera.get_vector("blue", "red")
        self.camera.get_vector("green", "middle")
        self.camera.get_vector("middle", "yellow")
        self.main_vect = self.camera.color_vector['red-blue']
        self.target_vect = self.camera.color_vector['middle-yellow']
        self.conveyor_vect = self.camera.color_vector['middle-green']
        self.vectors = {'main': self.main_vect,
                        'conveyor': self.conveyor_vect,
                        'target': self.target_vect}
        self.item_colors = {'conveyor': 'green',
                            'target': 'yellow'}

    def speed_define(self, duration=700, perspective=False):
        self.get_params(perspective)
        self.camera.show(2000)
        start_point = self.camera.color_location["middle"]
        print("CHECKED")
        self.brokker.set_route(f"forward {duration}")
        self.brokker.enable_movement()
        while self.brokker.move_start.get_value():
            sleep(0.5)
        sleep(1)
        self.get_params(perspective)
        print("CHECKED")
        end_point = self.camera.color_location["middle"]
        self.camera.show(2000)
        vect = [end_point[0]-start_point[0], end_point[1]-start_point[1]]
        print(f"star - {start_point}\nend - {end_point}")
        norm = np.linalg.norm(vect)
        print(f"norm - {norm}")
        self.speed = norm / duration
        # if self.speed < 1:
        #     self.speed = 1

    def rotattion_define(self, duration=200, perspective=False):
        self.get_params(perspective)
        print("CHECKED")
        start = self.main_vect
        self.camera.show(2000)
        self.brokker.set_route(f"right {duration}")
        self.brokker.enable_movement()
        while self.brokker.move_start.get_value():
            sleep(0.5)
        sleep(1)
        self.get_params(perspective)
        print("CHECKED")
        end = self.main_vect
        self.camera.show(2000)
        norm_start = np.linalg.norm(start)
        norm_end = np.linalg.norm(end)
        chisl = np.dot(end, start)
        cos = chisl/(norm_end*norm_start)
        cos = np.clip(cos, -1.0, 1.0)
        angle = np.degrees(np.arccos(cos))
        self.rotation = angle/duration

    def move_define(self, target='yellow', perspective=False):
        self.get_params(perspective)
        middle = self.camera.color_location["middle"]
        target = self.camera.color_location[target]
        vect = [target[0] - middle[0], target[1] - middle[1]]
        path = np.linalg.norm(vect)
        print(f"path {path}")
        duration = int(path/self.speed)
        cmd = f"forward {duration}"
        return cmd

    def turn_define(self, target='target', perspective=False):
        self.get_params(perspective)
        bot_view = self.main_vect
        bot_target = self.vectors[target]
        cross = np.cross(bot_view, bot_target)
        if cross is None:
            print(None)
            return "left 0"
        if cross > 0:
            turn = "right"
        else:
            turn = "left"
        chisl = bot_view[0]*bot_target[0] + bot_view[1]*bot_target[1]
        cos = chisl/(np.linalg.norm(bot_target)*np.linalg.norm(bot_view))
        cos = np.clip(cos, -1.0, 1.0)
        angle = np.degrees(np.arccos(cos))
        duration = int(angle/self.rotation)
        cmd = f"{turn} {duration}"
        print(cmd)
        return cmd

    def commander(self, target=['target', 'yellow'], perspective=False):
        self.get_params(perspective)
        cmd = [f"{self.turn_define(target[0], perspective)}",
               # |||||| HERE
               f"{self.move_define(target[1], perspective)}"]
        for msg in cmd:
            self.brokker.set_route(msg)
            self.brokker.enable_movement()
            while self.brokker.move_start.get_value():
                sleep(0.5)

    def accurency_define(self, perspective=False):
        self.get_params(perspective)
        bot_lenght = np.linalg.norm(self.camera.color_vector["red-blue"])
        self.accurency = int(bot_lenght)

    def get_target_point(self, target, perspective=False):
        self.get_params(perspective)
        self.target_point = self.camera.color_location[target]

    def reached(self, target, perspective=False, acc=None):
        if acc is None:
            self.accurency_define()
            acc = self.accurency
        if target != "green":
            target = "yellow"
        self.get_params()
        bot = self.camera.color_location["middle"]
        target = self.target_point
        vect = [target[0]-bot[0], target[1]-target[1]]
        norm = np.linalg.norm(vect)
        return norm <= acc

    def calibration(self):
        self.setup()
        self.speed_define()
        self.rotattion_define()

    def suka(self):
        self.get_params()
        self.suka_points = [self.camera.color_location[c]
                            for c in self.camera.color_location.keys()]

        # if __name__ == "__main__":
        #     cam = Camera(0, "work")
        #     cam.configure()
        #     # brok = Mosquitto("test/topic", 'test/topic')
        #     brok = OpcServer("opc.tcp://0.0.0.0:4849")
        #     mv = Move(cam, brok)
        #     mv.calibration()
        #     while not mv.reached():
        #         mv.commander()
        #     print("DONE =)")


if __name__ == "__main__":
    cam = Camera(0, "work")
    cam.configure()
    brok = OpcServer("opc.tcp://0.0.0.0:4840")
    mv = Move(cam, brok)
    mv.start_video()
    import time
    t1 = time.time()
    t2 = t1
    while t2 - t1 < 20:
        print(mv.Video.is_alive())
        t2 = time.time()
        continue
    # cmd = "forward 10"
    # while cmd != "stop":
    #     cmd = list(cmd.split(";"))
    #     print(cmd)
    #     for i in cmd:
    #         brok.set_route(i)
    #         brok.enable_movement()
    #         wait = int(list(i.split())[1])
    #         while brok.move_start.get_value():
    #             sleep(0.2)

    #     cmd = input("input cmd \n ")
