#pragma once 
#include <mosquitto.h>
#include <string>

class Mqtt{
  const char* BROKER_ADDRESS = "localhost";
  const int BROKER_PORT = 1883;
  static const char* TOPIC;
  static const int QOS = 1;
  const char* CLIENT_ID = "windows_subscriber";
  struct mosquitto* mosq = nullptr;
  int rc;
  static void on_connect(struct mosquitto* mosq, void* userdata, int rc);
  static void on_message(struct mosquitto* mosq, void* userdata, const struct mosquitto_message* msg);

public:
  Mqtt();
  static std::string message;
  std::string get_message();
};