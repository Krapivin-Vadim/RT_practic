#include "Mqtt.h"

#include <mosquitto.h>
#include <cstring>
#include <iostream>

// Mqtt::BROKER_ADDRESS = "localhost";
// Mqtt::BROKER_PORT = 1883;
const char* Mqtt::TOPIC = "test/topic";
// Mqtt::QOS = 1;
// Mqtt::CLIENT_ID = "subscriber"
std::string Mqtt::message = "";

void Mqtt::on_connect(struct mosquitto* mosq, void* userdata, int rc){
  if (rc == 0) {
    std::cout << "Connected to broker!" << std::endl;
    mosquitto_subscribe(mosq, nullptr, TOPIC, QOS);
    }
  else {
      std::cerr << "Connection error: "
                << mosquitto_connack_string(rc)
                << std::endl;
    }
}

void Mqtt::on_message(struct mosquitto* mosq, void* userdata, const struct mosquitto_message* msg) {
    char* text = static_cast<char*>(msg->payload);
    for(unsigned int i = 0; i != msg->payloadlen; ++i){
      message.push_back(text[i]);
    }
}


Mqtt::Mqtt(){
  mosquitto_lib_init();
  mosq = mosquitto_new(CLIENT_ID, true, nullptr);

  if(!mosq){
    std::cout << "Faild to create client" << std::endl;
    return;
  }

  mosquitto_connect_callback_set(mosq, on_connect);
  mosquitto_message_callback_set(mosq, on_message);

  rc = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60);
  if(rc != MOSQ_ERR_SUCCESS){
    std::cout << "Connect error: " << mosquitto_strerror(rc) << std::endl;
    return;
  }

  std::cout << "Listening for messages on topic '" << TOPIC << "'..." << std::endl;
}

std::string Mqtt::get_message(){
  while(message.length() == 0){
    mosquitto_loop(mosq, 100, 1);
  }
  std::string output(message);
  message.shrink_to_fit();
  return output;
}
