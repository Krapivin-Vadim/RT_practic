cmake_minimum_required(VERSION 3.12)
project(Bot)
set(MOSQUITTO_INCLUDE_DIR "C:/Program Files/mosquitto/devel/")  # Путь к заголовкам
set(MOSQUITTO_LIBRARY_PATH "C:/Program Files/mosquitto/devel/mosquitto.lib")  # Полный путь к .lib

file(GLOB SOURCES "*.cpp")
add_executable(Bot Source.cpp 
                    ACmdReceiver.cpp
                    AEngine.cpp
                    ControlSystem.cpp
                    FooCmdReciver.cpp
                    FooEngine.cpp
                    Mqtt.cpp)

# Подключение заголовков
target_include_directories(Bot PUBLIC ${MOSQUITTO_INCLUDE_DIR})

# Линковка статической библиотеки
target_link_libraries(Bot PUBLIC ${MOSQUITTO_LIBRARY_PATH})