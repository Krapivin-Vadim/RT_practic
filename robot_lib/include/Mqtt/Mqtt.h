#pragma once 
#include <mosquitto.h>
#include <string>

class Mqtt{
  struct mosquitto* mosq;
  std::string topic;
  int rc;
  std::string buffer;
  std::string host;
  unsigned int port;
  static void on_connect(struct mosquitto* mosq, void* userdata, int rc);
  static void on_message(struct mosquitto* mosq, void* userdata, const struct mosquitto_message* msg);
  static void on_disconnect(struct mosquitto *mosq, void *obj, int rc);

public:
  Mqtt(std::string host, unsigned int port, std::string topic);
  ~Mqtt();
  void connect();
  std::string get_message();
  void send_message(std::string message);
};