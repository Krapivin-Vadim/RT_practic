// #include "Mqtt.h"
// #include <iostream>
// #include <cstring>

// Mqtt::Mqtt(const std::string& address, 
//            int port,
//            const std::string& topic,
//            const std::string& client_id)
//     : broker_address(address),
//       broker_port(port),
//       topic(topic),
//       client_id(client_id),
//       connected(false),
//       running(false) {
//     mosquitto_lib_init();
//     mosq = mosquitto_new(client_id.empty() ? nullptr : client_id.c_str(), true, this);
//     if(!mosq) {
//         throw std::runtime_error("Failed to create mosquitto instance");
//     }
//     mosquitto_connect_callback_set(mosq, on_connect_wrapper);
//     mosquitto_message_callback_set(mosq, on_message_wrapper);
// }

// Mqtt::~Mqtt() {
//     disconnect();
//     if(mosq) {
//         mosquitto_destroy(mosq);
//     }
//     mosquitto_lib_cleanup();
// }

// bool Mqtt::connect() {
//     int rc = mosquitto_connect(mosq, broker_address.c_str(), broker_port, 60);
//     if(rc != MOSQ_ERR_SUCCESS) {
//         std::cerr << "Connection error: " << mosquitto_strerror(rc) << std::endl;
//         return false;
//     }
//     running = true;
//     return true;
// }

// void Mqtt::disconnect() {
//     if(connected) {
//         mosquitto_disconnect(mosq);
//         connected = false;
//     }
//     running = false;
// }

// bool Mqtt::is_connected() const {
//     return connected.load();
// }

// std::string Mqtt::get_message() {
//     std::lock_guard<std::mutex> lock(msg_mutex);
//     std::string result = message;
//     message.clear();
//     return result;
// }

// bool Mqtt::publish(const std::string& payload, int qos) {
//     if(!connected) return false;
//     int rc = mosquitto_publish(mosq, nullptr, topic.c_str(), payload.size(), payload.c_str(), qos, false);
//     return rc == MOSQ_ERR_SUCCESS;
// }

// // Статические методы-обертки
// void Mqtt::on_connect_wrapper(struct mosquitto* mosq, void* userdata, int rc) {
//     Mqtt* instance = static_cast<Mqtt*>(userdata);
//     if(instance) instance->on_connect(rc);
// }

// void Mqtt::on_message_wrapper(struct mosquitto* mosq, void* userdata, const struct mosquitto_message* msg) {
//     Mqtt* instance = static_cast<Mqtt*>(userdata);
//     if(instance) instance->on_message(msg);
// }

// // Методы обработки событий
// void Mqtt::on_connect(int rc) {
//     if(rc == MOSQ_ERR_SUCCESS) {
//         connected = true;
//         mosquitto_subscribe(mosq, nullptr, topic.c_str(), 1);
//         std::cout << "Connected and subscribed to " << topic << std::endl;
//     } else {
//         std::cerr << "Connection failed: " << mosquitto_connack_string(rc) << std::endl;
//         connected = false;
//     }
// }

// void Mqtt::on_message(const struct mosquitto_message* msg) {
//     if(msg->payloadlen > 0) {
//         std::lock_guard<std::mutex> lock(msg_mutex);
//         message.assign(static_cast<char*>(msg->payload), msg->payloadlen);
//     }
// }

//#include "Mqtt.h"

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

Mqtt::~Mqtt(){
  mosquitto_disconnect(this->mosq);
  mosquitto_destroy(this->mosq);
  mosquitto_lib_cleanup();
}

