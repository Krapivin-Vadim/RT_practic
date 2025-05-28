#pragma once

#include <string>
#include <linux/gpio.h>
//#include <gpiod.h>

class Motor{
    int chip_fd;
    int line_fd;
    int enable_fd;
public:
    Motor(std::string device, unsigned int pin_1, unsigned int pin_2, unsigned int enablePin);
    ~Motor();
    void forward();
    void backward();
    void stop();
};