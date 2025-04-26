#include "ControlSystem.h"
#include <string>
#include <sstream>
#include <iostream>
#include <functional>


void ControlSystem::run(){
  string msg = Receiver.receive();
  list<Cmd> cmds = parseMessage(msg);
  while(cmds.size() > 0){
    Cmd cmd = cmds.front();
    std::cout << "Cmd >>>" << cmd.name << " " << cmd.args << std::endl;
    std::function<void(int)> foo;
    foo = Engine.cmd_list[cmd.name];
    foo(cmd.args);
    cmds.pop_front();
  }
}


list<Cmd> ControlSystem::parseMessage(string message){
  stringstream cmd_define(message);
  list<Cmd> out;
  Cmd cmd;
  while(cmd_define >> cmd.name >> cmd.args){
    out.push_back(cmd);
    cmd.args = 0;
    cmd.name.clear();
  }
  return out;
}