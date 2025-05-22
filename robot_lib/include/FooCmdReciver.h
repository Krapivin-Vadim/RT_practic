#pragma once

#include "ACmdReceiver.h"
#include "Mqtt/Mqtt.h"
#include <string>

using namespace std;

class FooCmdReceiver: public ACmdReceiver{
public:
  string receive(string addr, unsigned int port, string topic);
};