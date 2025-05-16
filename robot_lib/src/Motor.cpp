#include "../include/Motor.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <gpiod.h>
#include <sys/ioctl.h>

const char* chip = "/dev/gpiochip0";

Motor::Motor(const unsigned int pin_1, const unsigned int pin_2){
    chip_fd = open(chip, O_RDWR);
    if(!chip_fd){
        std::cout << "Open chip error";
        return;
    }
    struct gpiohandle_request req;
    std::memset(&req, 0, sizeof(req));
    req.lines = 2;
    req.lineoffsets[0] = pin_1; 
    req.lineoffsets[1] = pin_2; 
    req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    req.default_values[0] = req.default_values[1] = 0;
    if(ioctl(chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0){
        std::cout << "Line request error";
        return;
    }
    line_fd = req.fd;
}

Motor::~Motor(){
    close(line_fd);
    close(chip_fd);
}

void Motor::forward(){
    struct gpiohandle_data data;
    data.values[0] = 1;
    data.values[1] = 0;
    ioctl(line_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
}

void Motor::backward(){
    struct gpiohandle_data data;
    data.values[0] = 0;
    data.values[1] = 1;
    ioctl(line_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
}

void Motor::stop(){
    struct gpiohandle_data data;
    data.values[0] = 0;
    data.values[1] = 0;
    ioctl(line_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
}