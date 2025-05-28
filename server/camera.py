import cv2
import numpy as np
from keyboard import is_pressed
from typing import Tuple
# from copy import deepcopy
from os.path import abspath

PATH = '\\'.join([i for i in abspath(__file__).split("\\")[:-1]])
# cap = cv2.VideoCapture(web_cam, cv2.CAP_DSHOW)


def restart(func):
    def wrapper(*args, **kwargs):
        try:
            func(args, kwargs)
        except RetryExc:
            func(args, kwargs)
    return wrapper


class RetryExc(Exception):
    def __init__(self, func_name):
        super().__init__(func_name)


def void():
    pass


class Trackbar:
    def __init__(self, color: str):
        self.color = color
        cv2.namedWindow(color, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(color, 640, 240)
        cv2.createTrackbar("h_low", color, 0, 179, void)
        cv2.createTrackbar("h_high", color, 0, 179, void)
        cv2.createTrackbar("s_low", color, 0, 255, void)
        cv2.createTrackbar("s_high", color, 0, 255, void)
        cv2.createTrackbar("v_low", color, 0, 255, void)
        cv2.createTrackbar("v_high", color, 0, 255, void)

    def get_param(self, param: str) -> tuple([int, int]):
        param_low = cv2.getTrackbarPos(f"{param}_low", self.color)
        param_high = cv2.getTrackbarPos(f"{param}_high", self.color)
        return param_low, param_high

    def get_H(self) -> tuple([int, int]):
        return self.get_param("h")

    def get_S(self) -> tuple([int, int]):
        return self.get_param("s")

    def get_V(self) -> tuple([int, int]):
        return self.get_param("v")

    def get_lower(self) -> tuple([int, int, int]):
        h, s, v = self.get_H()[0], self.get_S()[0], self.get_V()[0]
        return h, s, v

    def get_upper(self):
        h, s, v = self.get_H()[1], self.get_S()[1], self.get_V()[1]
        return h, s, v


class Camera:
    def __init__(self, camera, mode="default"):
        if mode == "work":
            self.work = True
            self.red_bar = Trackbar('red')
            self.blue_bar = Trackbar("blue")
            self.green_bar = Trackbar("green")
            self.sycned = False

        self.cap = cv2.VideoCapture(camera)
        self.frame = None
        self.hsv = None
        self.color_location = {'blue': None,
                               'green': None,
                               'red': None,
                               'middle': None}
        self.color_vector = {
            'red-blue': None,
            'middle-green': None
        }

        self.angle = None
        self.H_matrix = None
        self.blue_lower = np.array([98, 100, 100])
        self.blue_upper = np.array([112, 255, 255])

        self.red_lower = np.array([150, 100, 100])
        self.red_upper = np.array([180, 255, 255])

        self.green_lower = np.array([40, 40, 40])
        self.green_upper = np.array([95, 255, 255])
        self.color_devides = {"blue": [self.blue_lower,
                                       self.blue_upper],
                              "red": [self.red_lower,
                                      self.red_upper],
                              "green": [self.green_lower,
                                        self.green_upper]}

        # green_lower = np.array([40, 50, 50])
        # green_upper = np.array([80, 255, 255])

    def __del__(self):
        self.cap.release()
        try:
            cv2.destroyAllWindows()
        except Exception:
            pass

    def sync(self):
        if not self.work:
            print("it is default mode")
            return

        np.copyto(self.blue_lower, self.blue_bar.get_lower())
        np.copyto(self.blue_upper, self.blue_bar.get_upper())
        np.copyto(self.green_lower, self.green_bar.get_lower())
        np.copyto(self.green_upper, self.green_bar.get_upper())
        np.copyto(self.red_lower, self.red_bar.get_lower())
        np.copyto(self.red_upper, self.red_bar.get_upper())
        self.sycned = True

    def color_def(self):
        if not self.work:
            print("it is default mode")
            return
        while True:
            self.sync()
            self.sycned = False
            self.imget()
            self.to_hsv()
            self.build_masks()
            for color in self.color_location.keys():
                if color == "middle":
                    continue
                self.get_color_location(color)
            self.show(1)
            self.bin_vis("blue", 1)
            self.bin_vis("red", 1)
            self.bin_vis("green", 1)
            if is_pressed("q"):
                break
        self.sync()

    def check_syched(self):
        if not self.work:
            print("it is default mode")
            return
        val = self.sycned
        if val:
            self.sycned = not self.sycned
        return val

    def imget(self):
        if not self.cap:
            print("Undefined cap")
            return
        ret, self.frame = self.cap.read()
        # print(self.frame[0])
        if not ret:
            print("frame error")

    def to_hsv(self):
        if self.frame is None:
            print("Undefined frame")
            return
        self.hsv = cv2.cvtColor(self.frame, cv2.COLOR_BGR2HSV)

    def build_masks(self):
        if self.hsv is None:
            print("Undefined hsv")
            return

        self.masks = {'blue': cv2.inRange(self.hsv, self.blue_lower, self.blue_upper),
                      'green': cv2.inRange(self.hsv, self.green_lower, self.green_upper),
                      'red': cv2.inRange(self.hsv, self.red_lower, self.red_upper)}

    def bin_vis(self, color, waitkey):
        # red_res = np.zeros_like(frame, dtype='uint8')
        # red_res[red_mask == 255] = [255, 255, 255]
        # cv2.imshow('red', red_res)
        mask = self.masks[color]
        res = np.zeros_like(self.frame, dtype='uint8')
        res[mask == 255] = [255, 255, 255]
        cv2.imshow(f"bin_{color}", res)
        cv2.waitKey(waitkey)

    def get_color_location(self, color: str):
        if color == 'middle':
            return
        color_contours,  _ = cv2.findContours(
            self.masks[color], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        if not color_contours:
            print("Find contour error")
            return

        max_contour = max(color_contours, key=cv2.contourArea)
        area = cv2.contourArea(max_contour)

        if area > 100:
            M = cv2.moments(max_contour)
            if M['m00'] != 0:
                cx = int(M['m10'] / M['m00'])
                cy = int(M['m01'] / M['m00'])
            else:
                x, y, w, h = cv2.boundingRect(max_contour)
                cx = x + w // 2
                cy = y + h // 2

            self.color_location[color] = (cx, cy)

    def get_middle_location(self):
        if self.color_location['blue'] is None or self.color_location['red'] is None:
            print("Undefind color location (middle)")
            return
        middle_x = int(
            (self.color_location['blue'][0] + self.color_location['red'][0]) / 2)
        middle_y = int(
            (self.color_location['blue'][1] + self.color_location['red'][1]) / 2)
        self.color_location['middle'] = (middle_x, middle_y)

    def show(self, wait):
        cv2.imshow('frame', self.frame)
        # cv2.waitKey(1)
        for color in self.color_location.keys():
            location = self.color_location[color]
            if location is None:
                continue
            location = [int(i) for i in location]
            cv2.circle(self.frame, location, 5,
                       (0, 0, 255), 1)  # Probably '-1'
            cv2.putText(self.frame, f'{location[0]}:{location[1]}',
                        (location[0]+10, location[1]-10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1)
        cv2.imshow('frame', self.frame)
        cv2.waitKey(wait)

    def get_vector(self, color1: str, color2: str):
        key = ''
        if color1 == 'blue' or color2 == 'blue':
            key = 'red-blue'
            end = self.color_location['blue']
            begin = self.color_location['red']
        else:
            key = 'middle-green'
            end = self.color_location['green']
            begin = self.color_location['middle']
        if not begin or not end:
            print('Undefind color location (vector)')
            return
        vect_x = end[0] - begin[0]
        vect_y = end[1] - begin[1]
        vector = (vect_x, vect_y)
        self.color_vector[key] = vector

    def get_angle(self):
        for vect in self.color_vector.keys():
            if self.color_vector[vect] is None:
                print(f'Undefind vector {vect}')
                return
        module_rb = np.linalg.norm(self.color_vector['red-blue'])
        module_mg = np.linalg.norm(self.color_vector['middle-green'])
        x = self.color_vector['red-blue'][0] * \
            self.color_vector['middle-green'][0]
        y = self.color_vector['red-blue'][1] * \
            self.color_vector['middle-green'][1]
        cos = (x + y) / (module_mg * module_rb)
        self.angle = np.degrees(np.arccos(cos))
        return [cos, self.angle]

    @restart
    def dynamic_calibration(self, color):
        srcPoints = []
        for i in range(4):
            print("Start new point detection? (y/n)")
            cmd = input()
            if cmd == "n":
                cmd = input("Restart calibration? (y/n)")
                if cmd == "y":
                    raise RetryExc
                else:
                    return
            self.get_color_location(color)
            srcPoints.append(self.color_locationp[color])

        print("Input true points' coordinates")
        truePoints = list(map(int, input().split()))

        self.H_matrix, _ = cv2.findHomography(
            srcPoints, truePoints, cv2.RANSAC, 5.0)

    def static_calibration(self, color):
        srcdataset = [
            rf"{PATH}\true_dataset\photo-{num}.jpg" for num in range(1, 5)]
        no_srcdataset = [
            rf"{PATH}\perspective_dataset\photo-{num}.jpg" for num in range(1, 5)]

        src_points = []
        no_src_points = []
        for file in srcdataset:
            self.frame = cv2.imread(file)
            self.to_hsv()
            self.build_masks()
            self.get_color_location(color)
            src_points.append(list(self.color_location[color]))

        for file in no_srcdataset:
            self.frame = cv2.imread(file)
            self.to_hsv()
            self.build_masks()
            self.get_color_location(color)
            no_src_points.append(list(self.color_location[color]))

        src_points = np.array(src_points, dtype=np.float32)
        no_src_points = np.array(no_src_points, dtype=np.float32)
        self.H_matrix, _ = cv2.findHomography(
            no_src_points, src_points, cv2.RANSAC, 3.0)

    def fix_location(self):
        if self.H_matrix is None:
            print("Undefined Homography matrix")
            return
        all_locations = [self.color_location, self.color_vector]
        for item in all_locations:
            for key in item.keys():
                if item[key] is None:
                    continue

                item_3_dim = np.append(item[key], 1)
                res = np.dot(self.H_matrix, item_3_dim)
                res = res / res[2]
                res = res[:2]
                item[key] = res

    def save_settings(self):
        def in_file(devides):
            low = ' '.join([str(i) for i in devides[0]])
            upper = ' '.join([str(i) for i in devides[1]])
            a = f"[{low}, {upper}]\n"
            return a

        with open("camera.conf", "w") as config:
            for color in self.color_devides.keys():
                config.write(f'{color}:{in_file(self.color_devides[color])}')

    def configure(self):
        with open("camera.conf", "r") as config:
            for string in config:
                string = string.strip()
                color = string[:string.index(":")]
                low = string[string.index("[") + 1: string.index(",")]
                low = list(map(int, low.split()))
                high = string[string.index(",")+1: string.index("]")]
                high = list(map(int, high.split()))
                np.copyto(self.color_devides[color][0], low)
                np.copyto(self.color_devides[color][1], high)


if __name__ == "__main__":
    cam = Camera(0, "work")
    cam.color_def()
    cam.save_settings()
    cam.configure()
