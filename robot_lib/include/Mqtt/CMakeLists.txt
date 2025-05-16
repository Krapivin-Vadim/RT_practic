cmake_minimum_required(VERSION 3.12)
if (WIN32)
    set(MOSQUITTO_INCLUDE_DIR "C:/Program Files/mosquitto/devel/")  # Путь к заголовкам
    set(MOSQUITTO_LIBRARY_PATH "C:/Program Files/mosquitto/devel/mosquitto.lib")  # Полный путь к .lib
elseif(UNIX)
    set(MOSQUITTO_INCLUDE_DIR "/usr/include")  # Путь к заголовкам
    set(MOSQUITTO_LIBRARY_PATH "/usr/lib/x86_64-linux-gnu/libmosquitto.so")  # Полный путь к .lib
endif()

add_library(Mqtt STATIC Mqtt.cpp)

# Подключение заголовков
target_include_directories(Mqtt PUBLIC ${MOSQUITTO_INCLUDE_DIR})

# Линковка статической библиотеки
target_link_libraries(Mqtt PUBLIC ${MOSQUITTO_LIBRARY_PATH})