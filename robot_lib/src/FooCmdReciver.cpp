#include "FooCmdReciver.h"
#include <iostream>
#include <string>

string FooCmdReceiver::receive(std::string addr, unsigned int port, std::string topic){
  cout << "in cmd receiver\n";
  Mqtt Subs(addr, port, topic);
  Subs.connect();
  return Subs.get_message();
}