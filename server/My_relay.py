from time import sleep

import pyhid_usb_relay as usbr


class Conveyor:
    def __init__(self):
        self.relay = usbr.find()

    def execute(self, duration):
        self.relay[1] = 1
        sleep(duration)
        self.relay[1] = 0


if __name__ == "__main__":
    REL_NUM = 1
    relay = usbr.find()
    print(relay)
    num = relay.num_relays
    relay[REL_NUM] = 1
    sleep(2)
    relay[REL_NUM] = 0
