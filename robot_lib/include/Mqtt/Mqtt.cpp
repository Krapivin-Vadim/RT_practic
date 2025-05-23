#include "Mqtt.h"
#include <mosquitto.h>
#include <iostream>


// Callback-функция при подключении
void Mqtt::on_connect(struct mosquitto *mosq, void *obj, int rc) {
    if (rc == 0) {
        printf("Подключено к брокеру\n");
        // Подписываемся на топик после подключения
        mosquitto_subscribe(mosq, NULL, static_cast<Mqtt*>(obj)->topic.c_str(), 0);
    } else {
        fprintf(stderr, "Ошибка подключения: %s\n", mosquitto_connack_string(rc));
    }
}

//Callback-функция при получении сообщения
void Mqtt::on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    if (msg->payloadlen > 0) {
        static_cast<Mqtt*>(obj)->buffer = static_cast<char*>(msg->payload);
        std::cout << "Получено сообщение: " << (char*)msg->payload << std::endl;
        // Инициируем отключение после получения сообщения
        mosquitto_disconnect(mosq);
    }
}

// Callback-функция при отключении
void Mqtt::on_disconnect(struct mosquitto *mosq, void *obj, int rc) {
    std::cout << "Отключено от брокера\n";
    mosquitto_loop_stop(mosq, true); // Останавливаем цикл событий
}

Mqtt::Mqtt(std::string Host, unsigned int Port, std::string Topic){
    this->host = Host;
    this->port = Port;
    this->topic = Topic;
    // Инициализация библиотеки
    mosquitto_lib_init();

    // Создание клиента
    mosq = mosquitto_new(NULL, true, this);
    if (!mosq) {
        std::cout << "Ошибка создания клиента\n";
        std::cout << stderr << std::endl;
        return;
    }

    // Установка callback-функций
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
}

Mqtt::~Mqtt(){
    mosquitto_destroy(this->mosq);
    mosquitto_lib_cleanup();
}

void Mqtt::connect(){
    this->rc = mosquitto_connect(mosq, this->host.c_str(), this->port, 60);
    if (this->rc != MOSQ_ERR_SUCCESS) {
        std::cout << "Не удалось подключиться: " << mosquitto_strerror(this->rc) << std::endl;
        return;
    }
    mosquitto_loop_forever(this->mosq, -1, 1);
}

std::string Mqtt::get_message(){
    return this->buffer;
}

void Mqtt::send_message(std::string message){
    
    struct mosquitto* pub_mosq = mosquitto_new(NULL, true, this);
    if (!pub_mosq) {
        std::cout << "Ошибка создания клиента\n";
        std::cout << stderr << std::endl;
        return;
    }
    int pub_rc = mosquitto_connect(pub_mosq, this->host.c_str(), this->port, 60);
    if (pub_rc != MOSQ_ERR_SUCCESS) {
        std::cout << "Не удалось подключиться: " << mosquitto_strerror(this->rc) << std::endl;
        return;
    }

    mosquitto_publish(pub_mosq, nullptr, this->topic.c_str(), message.size(), message.c_str(), 0, false);
    mosquitto_disconnect(pub_mosq);
    mosquitto_destroy(pub_mosq);
    mosquitto_lib_cleanup();

}