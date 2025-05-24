#include "../include/MainEngine.h"
//#include <unistd.h>
#include <thread>
#include <chrono>

void sleep(int time_ms){
  std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
}

MainEngine::MainEngine() : left_motor(12, 13), right_motor(20, 21){
  cmd_list["forward"] = [this](int x){forward(x);};
  cmd_list["left"] = [this](int x){left(x);};
  cmd_list["right"] = [this](int x){right(x);};
}

void MainEngine::forward(int time_ms){
  left_motor.forward();
  right_motor.forward();
  sleep(time_ms);
  left_motor.stop();
  right_motor.stop();
}

void MainEngine::right(int time_ms){
  left_motor.backward();
  right_motor.forward();
  sleep(time_ms);
  left_motor.stop();
  right_motor.stop();
}

void MainEngine::left(int time_ms){
  left_motor.forward();
  right_motor.backward();
  sleep(time_ms);
  left_motor.stop();
  right_motor.stop();
}

void MainEngine::stop(){
  //empty method
}
