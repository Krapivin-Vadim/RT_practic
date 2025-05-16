cmake_minimum_required(VERSION 3.12)

# set(MOSQUITTO_INCLUDE_DIR "C:/Program Files/mosquitto/devel/mosquitto.h")  # Путь к заголовкам
# set(MOSQUITTO_LIBRARY_PATH "C:/Program Files/mosquitto/devel/mosquitto.lib")  # Полный путь к .lib
add_subdirectory(include/Mqtt/)
file(GLOB SOURCES "src/*.cpp")
include_directories("include/")
add_library(Robot STATIC ${SOURCES})
target_link_libraries(Robot PUBLIC Mqtt)

# # Подключение заголовков
# target_include_directories(Robot_2_0 PRIVATE ${MOSQUITTO_INCLUDE_DIR})

# # Линковка статической библиотеки
# target_link_libraries(Robot_2_0 PRIVATE ${MOSQUITTO_LIBRARY_PATH})