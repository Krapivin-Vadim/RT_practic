#include "robot_lib/include/ControlSystem.h"
#include <iostream>
#include <string>
using namespace std;
int main(){
    string topic; string host; int port; string mode; 
    cout << "Entrer broker ip >>>   "; getline(cin, host); cout << endl;
    cout << "Entrer topic >>>   "; getline(cin, topic); cout << endl;
    cout << "Entrer workong mode >>>   "; getline(cin, mode); cout << endl;
    cout << "Entrer port >>>   "; cin >> port; cout << endl;
    ControlSystem Sys(host, port, topic, mode);
    Sys.run();
    while(Sys.isrunnig()){
        Sys.run();
    }
    std::cout << "Finish\n";
    return 0;
}