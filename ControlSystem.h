#pragma once
#include "FooEngine.h"
#include "FooCmdReciver.h"

struct Cmd{
  string name;
  int args;
};

class ControlSystem{
  FooEngine Engine;
  FooCmdReceiver Receiver;
  Cmd parseMessage(string message);
  public:
  void run();
};