import paho.mqtt.client as mqtt
import time


def my_connect(topic):

    def on_connect(client, userdata, flags, reason_code, properties):
        client.subscribe(topic)

    return on_connect


def my_message(mosquitto_object):

    def on_message(client, userdata, msg):
        mosquitto_object.buffer = msg.payload.decode()
        client.disconnect()

    return on_message


class Mosquitto_2:
    def __init__(self, pub_topic, host="localhost", port=1883):
        self.buffer = None
        self.host = host
        self.port = port
        self.publisher = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        # self.publisher.connect(host, port)
        self.pub_topic = pub_topic
        self.publisher.on_connect = my_connect(pub_topic)
        self.publisher.on_message = my_message(self)
        # self.subscriber = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        # self.subs_topic = subs_topic
        # self.subscriber.on_connect = my_connect(subs_topic)
        # self.subscriber.on_message = my_message(self)

    def __del__(self):
        self.publisher.disconnect()

    def pub(self, message):
        self.publisher.connect(self.host, self.port)
        self.publisher.publish(self.pub_topic, message)

    def get_messsage(self):
        self.publisher.connect(self.host, self.port)
        self.publisher.loop_forever(15, 1)
        print(self.buffer)


class Mosquitto:
    def __init__(self, pub_topic, subs_topic, host="localhost", port=1883):
        self.buffer = None
        self.host = host
        self.port = port
        self.publisher = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        self.publisher.connect(host, port)
        self.pub_topic = pub_topic
        self.subscriber = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        self.subs_topic = subs_topic
        self.subscriber.on_connect = my_connect(subs_topic)
        self.subscriber.on_message = my_message(self)

    def pub(self, message):
        self.publisher.publish(self.pub_topic, message)

    def get_messsage(self):
        self.subscriber.connect(self.host, self.port)
        self.subscriber.loop_forever(10)
        print(self.buffer)


if __name__ == "__main__":
    M = Mosquitto('test/topic', 'test/topic')
    print(1)
    M.pub("forward 10")
    print(2)
    M.get_messsage()
    print(3)
    M.pub("stop 0")
    print(4)
    M.get_messsage()
