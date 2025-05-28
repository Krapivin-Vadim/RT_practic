//#include "../include/Motor.h"
#include "Motor.h"

#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <string>



    
Motor::Motor(std::string device, unsigned int pin_1, unsigned int pin_2, unsigned int enablePin) {
    std::string format = "/dev/gpiochip" + device;
    const char* chip = format.c_str();

    // Открываем основной чип
    chip_fd = open(chip, O_RDWR);
    if (chip_fd < 0) {
        std::cerr << "Error opening chip: " << strerror(errno) << std::endl;
        return;
    }

    // Настраиваем основные пины
    struct gpiohandle_request req;
    memset(&req, 0, sizeof(req));
    req.lineoffsets[0] = pin_1;
    req.lineoffsets[1] = pin_2;
    req.lines = 2;
    req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    
    if (ioctl(chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0) {
        std::cerr << "Line request error: " << strerror(errno) << std::endl;
        close(chip_fd);
        return;
    }
    line_fd = req.fd;

    // Настраиваем enable-пин
    int enable_chip_fd = open(chip, O_RDWR);
    if (enable_chip_fd < 0) {
        std::cerr << "Error opening enable chip: " << strerror(errno) << std::endl;
        close(chip_fd);
        close(line_fd);
        return;
    }

    struct gpiohandle_request enable_req;
    memset(&enable_req, 0, sizeof(enable_req));
    enable_req.lineoffsets[0] = enablePin;
    enable_req.lines = 1;
    enable_req.flags = GPIOHANDLE_REQUEST_OUTPUT;

    if (ioctl(enable_chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &enable_req) < 0) {
        std::cerr << "Enable line request error: " << strerror(errno) << std::endl;
        close(enable_chip_fd);
        close(chip_fd);
        close(line_fd);
        return;
    }
    enable_fd = enable_req.fd;
    close(enable_chip_fd); // Закрываем дескриптор чипа, так как line handle получен

    // Активируем мотор
    struct gpiohandle_data en_data;
    en_data.values[0] = 1;
    if (ioctl(enable_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &en_data) < 0) {
        std::cerr << "Error activating motor: " << strerror(errno) << std::endl;
    }
}

Motor::~Motor() {
    close(enable_fd);
    close(line_fd);
    close(chip_fd);
}


void Motor::forward(){
    struct gpiohandle_data data;
    data.values[1] = 1;
    data.values[0] = 0;
    ioctl(line_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
}

void Motor::backward(){
    struct gpiohandle_data data;
    data.values[1] = 0;
    data.values[0] = 1;
    ioctl(line_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
}

void Motor::stop(){
    struct gpiohandle_data data;
    data.values[0] = 0;
    data.values[1] = 0;
    ioctl(line_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
}