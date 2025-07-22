from opcua import Server


class OpcServer:
    def __init__(self, Url):
        self.URL = Url
        self.server = Server()
        self.server.set_endpoint(self.URL)
        self.ns = self.server.register_namespace("Move")
        self.objects = self.server.get_objects_node()
        self.my_device = self.objects.add_object(self.ns, "Move")
        self.move_start = self.my_device.add_variable(
            self.ns, "Enable move", False)
        self.route = self.my_device.add_variable(
            self.ns, "Route to target", "")
        self.move_start.set_writable(True)

    def __del__(self):
        self.server.stop()

    def start(self):
        self.server.start()

    def set_route(self, route):
        self.route.set_value(route)

    def enable_movement(self):
        self.move_start.set_value(True)


if __name__ == "__main__":
    serv = OpcServer("opc.tcp://0.0.0.0:4841")
    # server = Server()
    # server.set_endpoint("opc.tcp://0.0.0.0:4840/")
    # ns = server.register_namespace("MOVEMENT")
    # objects = server.get_objects_node()
    # my_device = objects.add_object(ns, "MOVE")
    # enable = False
    # move_start = my_device.add_variable(ns, "Enable movement", enable)
    # route = my_device.add_variable(ns, "Route to target", "forward 10 right 15")
    # move_start.set_writable(True)
    # try:
    #     server.start()
    #     print("Сервер запущен. Ctrl+C для остановки")
    #     while True:
    #         time.sleep(3)
    #         inp = input("Enable movement")
    #         if inp != 'C':
    #             continue
    #         node = server.get_node("ns=2;i=2")
    #         node.set_value(True)

    # finally:
    #     server.stop()
