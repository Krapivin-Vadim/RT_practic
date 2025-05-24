#include "ControlSystem.h"
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include "FooEngine.h"
#include "MainEngine.h"
#include "../include/Mqtt/Mqtt.h"

string work_mode = "work";

ControlSystem::ControlSystem(string Addr, unsigned int Port, string Topic, string mode){
  this->addres = Addr;
  this->port = Port;
  this->topic = Topic;
    if (mode == work_mode){
      cout << "work\n";
      this->Engine = new MainEngine;
    }
    else{
      cout << "debug\n";
      this->Engine = new FooEngine;
    }
    this->running = false;

  std::cout << "Constructed\n";  
}

ControlSystem::~ControlSystem(){
  delete this->Engine;
}

void ControlSystem::run(){
  cout << "in run \n";
  this->running = true;
  string msg = Receiver.receive(this->addres, this->port, this->topic);
  list<Cmd> cmds = parseMessage(msg);
  while(cmds.size() > 0){
    Cmd cmd = cmds.front();
    std::cout << "Cmd >>>" << cmd.name << " " << cmd.args << std::endl;
    if (cmd.name != "stop"){
      std::function<void(int)> foo;
      foo = Engine->cmd_list[cmd.name];
      foo(cmd.args);
    }
    else{
      this->running = false;
    }
    cmds.pop_front();
  }
  cout << "stop run\n";
  Mqtt answear(this->addres, this->port, this->topic);
  answear.send_message("done");
}

bool ControlSystem::isrunnig(){
  cout << "in isrunning\n";
  return this->running;
}

list<Cmd> ControlSystem::parseMessage(string message){
  cout << "in parseMessage\n";
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