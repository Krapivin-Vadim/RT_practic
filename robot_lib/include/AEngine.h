#pragma once

using namespace std;

class AEngine{
  virtual void forward(int time_ms);
  virtual void right(int time_ms);
  virtual void left(int time_ms);
  //virtual void stop();
};