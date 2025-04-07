#include "AEngine.h"
#include <iostream>

void AEngine::forward(int time_ms){
  std::cout << "This is an abstract class";
}
void AEngine::right(int time_ms){
  std::cout << "This is an abstract class";
}
void AEngine::left(int time_ms){
  std::cout << "This is an abstract class";
}
void AEngine::stop(){
  std::cout << "This is an abstract class";
}
