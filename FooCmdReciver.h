#pragma once

#include "ACmdReceiver.h"
#include "Mqtt.h"


class FooCmdReceiver: public ACmdReceiver{
public:
  string receive() override;
};