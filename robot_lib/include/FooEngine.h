#pragma once
#include "AEngine.h"
#include <string>
#include <unordered_map>
#include <functional>

class FooEngine: public AEngine{
public:
  FooEngine();
  std::unordered_map<std::string, std::function<void(int)>> cmd_list;
  void forward(int time_ms) override;
  void left(int time_ms) override;
  void right(int time_ms) override;
  void stop() override;
};