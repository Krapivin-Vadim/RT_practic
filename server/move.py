from camera import Camera
from mosquitto import Mosquitto
import numpy as np
import cv2


class Move:
    def __init__(self, camera: Camera, brokker: Mosquitto):
        self.camera = camera
        self.brokker = brokker

    def setup(self):
        self.camera.imget()
        self.camera.to_hsv()
        self.camera.build_masks()

    def get_params(self):
        self.setup()
        for color in self.camera.color_location.keys():
            self.camera.get_color_location(color)
        self.camera.get_middle_location()
        self.camera.get_vector("blue", "red")
        self.camera.get_vector("green", "middle")
        self.main_vect = self.camera.color_vector['red-blue']
        self.target_vect = self.camera.color_vector['middle-green']

    def right(self):
        if 95 > self.angle and self.angle < 180:
            return True
        return False  # left

    def test_speed(self, color, duration=1000):
        self.get_params()
        start_point = self.camera.color_location[color]
        input()
        self.get_params()
        end_point = self.camera.color_location[color]
        input()
        vect = [end_point[0]-start_point[0], end_point[1]-start_point[1]]
        print(f"star - {start_point}\nend - {end_point}")
        norm = np.linalg.norm(vect)
        print(f"norm - {norm}")
        self.speed = norm / duration
        print(f"speed = {self.speed}")

    def speed_define(self, duration=700):
        self.get_params()
        start_point = self.camera.color_location["middle"]

        self.brokker.pub(f"forward {duration}")
        self.camera.show(2000)
        # self.brokker.get_messsage()
        self.get_params()
        end_point = self.camera.color_location["middle"]
        self.camera.show(2000)
        vect = [end_point[0]-start_point[0], end_point[1]-start_point[1]]
        print(f"star - {start_point}\nend - {end_point}")
        norm = np.linalg.norm(vect)
        print(f"norm - {norm}")
        self.speed = norm / duration
        # if self.speed < 1:
        #     self.speed = 1

    def rotattion_define(self, duration=200):
        self.get_params()
        start = self.main_vect

        self.brokker.pub(f"right {duration}")
        self.camera.show(2000)
        # self.brokker.get_messsage()
        self.get_params()
        end = self.main_vect
        self.camera.show(2000)
        norm_start = np.linalg.norm(start)
        norm_end = np.linalg.norm(end)
        chisl = np.dot(end, start)
        cos = chisl/(norm_end*norm_start)
        cos = np.clip(cos, -1.0, 1.0)
        angle = np.degrees(np.arccos(cos))
        self.rotation = angle/duration

    def move_define(self):
        self.get_params()
        middle = self.camera.color_location["middle"]
        target = self.camera.color_location["green"]
        vect = [target[0] - middle[0], target[1] - middle[1]]
        path = np.linalg.norm(vect)
        print(f"path {path}")
        duration = int(path/self.speed)
        cmd = f"forward {duration}"
        return cmd

    def turn_define(self):
        self.get_params()
        bot_view = self.main_vect
        bot_target = self.target_vect
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

    def commander(self):
        self.get_params()
        cmd = f"{self.turn_define()} {self.move_define()}"
        self.brokker.pub(cmd)
        self.brokker.get_messsage()

    def reached(self, acc=100):
        self.get_params()
        bot = self.camera.color_location["middle"]
        target = self.camera.color_location["green"]
        vect = [target[0]-bot[0], target[1]-target[1]]
        norm = np.linalg.norm(vect)
        return norm == acc

    def calibration(self):
        mv.setup()
        mv.speed_define()
        mv.rotattion_define()


if __name__ == "__main__":
    cam = Camera(0, "work")
    cam.configure()
    brok = Mosquitto("test/topic", 'test/topic')
    mv = Move(cam, brok)
    mv.calibration()
    while not mv.reached():
        mv.commander()
    print("DONE =)")
