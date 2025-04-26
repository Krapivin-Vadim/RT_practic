#pragma once

#include <linux/gpio.h>
#include <gpiod.h>

class Motor{
    int chip_fd;
    int line_fd;
public:
    Motor(unsigned int pin_1, unsigned int pin_2);
    ~Motor();
    void forward();
    void backward();
    void stop();
};