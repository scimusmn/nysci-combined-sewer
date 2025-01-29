#include "flow.h"
#include "controller.h"
#include "timeout.h"


#define MAIN_OUTPUT_MAX 230
#define OVERFLOW_OUTPUT_MAX 330


Timeout t;
void drainAll(void*) {
  mainDrain->setDraining();
  overflowDrain->setDraining();
}

void setup() {
  controllerSetup(6);
  mainDrain->setOverflowing();
  overflowDrain->setOverflowing();
}

void loop() {
  // t.update();
  if (mainDrain->getOutputCount() > MAIN_OUTPUT_MAX) {
    mainDrain->setOverflowing();
  } else {
    mainDrain->setDraining();
  }
  if (overflowDrain->getOutputCount() > OVERFLOW_OUTPUT_MAX) {
    overflowDrain->setOverflowing();
  } else {
    overflowDrain->setDraining();
  }
  controllerLoop(false);
}
