#pragma once

#include "FooEngine.h"
#include "Motor.h"

class MainEngine: public FooEngine{
    Motor left_motor;
    Motor right_motor;
public:
    MainEngine();
    std::unordered_map<std::string, std::function<void(int)>> cmd_list;
    void forward(int time_ms) override;
    void left(int time_ms) override;
    void right(int time_ms) override;
    void stop() override;
};