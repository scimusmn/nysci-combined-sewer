#pragma once

#include "flow.h"
#include "messages.h"

void controllerSetup(uint8_t canBusId);
void controllerLoop(bool debug);

extern void (*extraProcessInputLevels)(InputLevels &levels);
