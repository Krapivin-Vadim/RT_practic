#include "FooCmdReciver.h"
#include <iostream>

string FooCmdReceiver::receive(){
  // std::string cmd;
  // std::getline(cin, cmd);
  // std::cout << ">>> " << cmd << endl;
  Mqtt Subs;
  return Subs.get_message();
}