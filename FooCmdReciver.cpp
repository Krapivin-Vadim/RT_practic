#include "FooCmdReciver.h"
#include <iostream>

string FooCmdReceiver::receive(){
  Mqtt Subs;
  return Subs.get_message();
}