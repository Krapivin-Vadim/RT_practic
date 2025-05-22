set(CMAKE_TOOLCHAIN_FILE arm-toolchain.cmake)
cmake_minimum_required(VERSION 3.12)

# set(CMAKE_C_COMPILER /usr/bin/gcc)
# set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(Bot_2)
add_subdirectory(robot_lib)
add_executable(Bot_2 Source.cpp)
target_link_libraries(Bot_2 PUBLIC Robot)