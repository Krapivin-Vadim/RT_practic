#include "FooEngine.h"
#include <iostream>
#include <Windows.h>
#include <string>



FooEngine::FooEngine(){
  cmd_list["forward"] = [this](int x){forward(x);};
  cmd_list["left"] = [this](int x){left(x);};
  cmd_list["right"] = [this](int x){right(x);};
}

void FooEngine::forward(int time_ms) {
  std::cout << "forward " << time_ms << endl;
  Sleep(time_ms);
}

void FooEngine::left(int time_ms) {
  std::cout << "left" << endl;
  Sleep(time_ms);
}

void FooEngine::right(int time_ms) {
  std::cout << "right" << endl;
  Sleep(time_ms);
}

void FooEngine::stop() {
  std::cout << "stop" << endl;
}