import cv2
import numpy as np
from copy import deepcopy
from os.path import abspath

PATH = '\\'.join([i for i in abspath(__file__).split("\\")[:-1]])


class Camera:
    def __init__(self, camera):
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

    def __del__(self):
        self.cap.release()
        try:
            cv2.destroyAllWindows()
        except Exception:
            pass

    def imget(self):
        if not self.cap:
            print("Undefined cap")
            return
        ret, self.frame = self.cap.read()

    def to_hsv(self):
        if self.frame is None:
            print("Undefined frame")
            return
        self.hsv = cv2.cvtColor(self.frame, cv2.COLOR_BGR2HSV)

    def build_masks(self):
        if self.hsv is None:
            print("Undefined hsv")
            return

        blue_lower = np.array([98, 100, 100])
        blue_upper = np.array([112, 255, 255])

        red_lower = np.array([150, 100, 100])
        red_upper = np.array([165, 255, 255])

        green_lower = np.array([40, 50, 50])
        green_upper = np.array([80, 255, 255])

        self.masks = {'blue': cv2.inRange(self.hsv, blue_lower, blue_upper),
                      'green': cv2.inRange(self.hsv, green_lower, green_upper),
                      'red': cv2.inRange(self.hsv, red_lower, red_upper)}

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

    def show(self):
        for color in self.color_location.keys():
            location = self.color_location[color]
            if location is not None:
                continue
            cv2.circle(self.frame, location, 5,
                       (0, 0, 255), 1)  # Probably '-1'
            cv2.putText(self.frame, f'{location[0]}:{location[1]}',
                        (location[0]+10, location[1]-10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1)
        cv2.imshow('frame', self.frame)
        cv2.waitKey()

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
        self.angle = np.arccos(cos)
        return [cos, self.angle]

    def calibration(self, color):
        srcPoints = []
        for i in range(4):
            print("Start new point detection?")
            cmd = input()
            self.get_color_location(color)
            srcPoints.append(self.color_locationp[color])

        print("Input true points' coordinates")
        truePoints = list(map(int, input().split()))

        self.H_matrix, _ = cv2.findHomography(
            srcPoints, truePoints, cv2.RANSAC, 5.0)

    def static_calibration(self, color):
        srcdataset = [
            rf"{PATH}\true_dataset\photo_2025-05-23_22-21-{num}.jpg" for num in range(1, 5)]
        no_srcdataset = [
            rf"{PATH}\perspective_dataset\photo_2025-05-23_22-20-{num}.jpg" for num in range(1, 5)]

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

        # # Only for test

        # fixed_location = []

        # for item in no_src_points:
        #     item = np.append(item, 1)
        #     res = np.dot(self.H_matrix, item)
        #     res = res / res[2]
        #     res = res[:2]
        #     fixed_location.append(res)

        # print(src_points, end='\n-----\n')
        # print(no_src_points, end='\n-----\n')
        # print(fixed_location)
        # frame = cv2.imread(
        #     rf"{PATH}\true_dataset\photo_2025-05-23_22-21-{4}.jpg")
        # for point in fixed_location:
        #     point = [int(i) for i in point]
        #     cv2.circle(frame, point, 5,
        #                (0, 0, 255), 1)  # Probably '-1'
        #     cv2.putText(frame, f'{point[0]}:{point[1]}',
        #                 (point[0]+10, point[1]-10),
        #                 cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1)
        # cv2.imshow("frame", frame)
        # cv2.waitKey(-1)

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


if __name__ == '__main__':

    test = Camera(0)
    test.static_calibration("blue")
    test.imget()
    img_frame = cv2.imread("C:\\Dev\\opencv_server\\test_4.jpg")
    test.frame = img_frame
    test.to_hsv()
    test.build_masks()

    for color in test.color_location.keys():
        test.get_color_location(color)

    test.get_middle_location()

    for vect in test.color_vector.keys():
        color1, color2 = vect.split('-')
        test.get_vector(color1, color2)

    test.fix_location()

    a = test.get_angle()

    test.show()
