#include "robot_lib/include/ControlSystem.h"
#include <iostream>

int main(){
    ControlSystem Sys("192.168.1.100", 1883, "test/topic", "work");
    Sys.run();
    while(Sys.isrunnig()){
        Sys.run();
    }
    std::cout << "Finish\n";
    return 0;
}