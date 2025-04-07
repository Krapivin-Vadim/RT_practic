#include "ControlSystem.h"
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

void ControlSystem::run(){
  string msg = Receiver.receive();
  Cmd cmd = parseMessage(msg);
  std::cout << "Cmd >>>" << cmd.name << " " << cmd.args << std::endl;
  if (cmd.args){
    std::function<void(int)> foo;
    foo = Engine.cmd_list[cmd.name];
    foo(cmd.args);
  }
  else{
    Engine.stop();
  }
}

Cmd ControlSystem::parseMessage(string message){
  stringstream cmd_define(message);
  Cmd cmd;
  cmd_define >> cmd.name >> cmd.args;
  return cmd;
}