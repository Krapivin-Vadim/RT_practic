#pragma once
#include "FooEngine.h"
#include "FooCmdReciver.h"
#include <list>
#include <string>

struct Cmd{
  string name;
  int args;
};

class ControlSystem{
  bool running;
  string addres;
  unsigned int port;
  string topic;
  FooEngine* Engine;
  FooCmdReceiver Receiver;
  list<Cmd> parseMessage(string message);
  public:
  void run();
  ControlSystem(string Addr, unsigned int Port, string Topic, string mode = "debug");
  //ControlSystem(const char* Addr, unsigned int Port, const char* Topic, const char* mode = "debug");
  ~ControlSystem();
  bool isrunnig();
  //void execute_command();
};