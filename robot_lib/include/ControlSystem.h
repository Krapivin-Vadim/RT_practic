#pragma once
#include "FooEngine.h"
#include "FooCmdReciver.h"
#include <list>

struct Cmd{
  string name;
  int args;
};

class ControlSystem{
  FooEngine Engine;
  FooCmdReceiver Receiver;
  list<Cmd> parseMessage(string message);
  public:
  void run();
  //void execute_command();
};