cmake_minimum_required(VERSION 3.12)

set(MOSQUITTO_INCLUDE_DIR "C:/Program Files/mosquitto/devel/")  # Путь к заголовкам
set(MOSQUITTO_LIBRARY_PATH "C:/Program Files/mosquitto/devel/mosquitto.lib")  # Полный путь к .lib

add_library(Mqtt STATIC Mqtt.cpp)

# Подключение заголовков
target_include_directories(Mqtt PUBLIC ${MOSQUITTO_INCLUDE_DIR})

# Линковка статической библиотеки
target_link_libraries(Mqtt PUBLIC ${MOSQUITTO_LIBRARY_PATH})