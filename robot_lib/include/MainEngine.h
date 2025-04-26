#pragma once

#include "FooEngine.h"
#include "Motor.h"

class MainEngine: public FooEngine{
    std::unordered_map<std::string, std::function<void(int)>> cmd_list;
    Motor left_motor;
    Motor right_motor;
    MainEngine();
    void forward(int time_ms) override;
    void left(int time_ms) override;
    void right(int time_ms) override;
    //void stop() override;
};