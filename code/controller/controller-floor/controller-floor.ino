#include "flow.h"
#include "controller.h"
#include "timeout.h"


Timeout t;
void drainAll(void*) {
  mainDrain->setDraining();
  overflowDrain->setDraining();
}

void setup() {
  controllerSetup(6);
  mainDrain->setOverflowing();
  overflowDrain->setOverflowing();
  t.set(60000, drainAll, nullptr);
}

void loop() {
  t.update();
  controllerLoop(false);
}
