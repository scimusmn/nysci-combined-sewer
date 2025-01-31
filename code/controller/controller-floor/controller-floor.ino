#include "flow.h"
#include "controller.h"
#include "timeout.h"


#define MAIN_OUTPUT_MAX 230
#define OVERFLOW_OUTPUT_MAX 330

#define CSO_TIME 20000
#define POONAMI_TIME 20000

#define CSO_RAIN 2
#define CSO_APPLIANCE 6
#define POONAMI_RAIN 3
#define POONAMI_APPLIANCE  9

struct EventState {
  uint8_t state = 0;
  bool drainable = true;
  Timeout timeout;
  void updateState(uint8_t raw) {
    state = ((state << 1) | raw) & 0x3;
  }
} cso, poonami;


bool updateEventsFlag = false;
void updateEvents(unsigned int rain, unsigned int appliances) {
  updateEventsFlag = false;
  cso.updateState(rain >= CSO_RAIN && appliances >= CSO_APPLIANCE);
  if (cso.state == 1) {
    cso.drainable = false;
    mainDrain->setOverflowing();
    cso.timeout.set(CSO_TIME, setTrue, &(cso.drainable));
  }

  poonami.updateState(rain >= POONAMI_RAIN && appliances >= POONAMI_APPLIANCE);
  if (poonami.state == 1) {
    poonami.drainable = false;
    poonami.timeout.set(POONAMI_TIME, setTrue, &(poonami.drainable));
  }
}


struct Holdable {
  unsigned int value;
  unsigned long holdTime = 4000;
  Timeout timeout;
  void update(unsigned int v) {
    Serial.println(value);
    if (v < value) {
      timeout.set(holdTime, Holdable::reset, this);
    } else if (v > value) {
      value = v;
      timeout.clear();
    }
  }
  static void reset(void *data) {
    Holdable *self = static_cast<Holdable*>(data);
    self->value = 0;
    updateEventsFlag = true;
  }
} rain, toilet, washer, dishes, shower;


void setTrue(void *data) {
  bool *b = static_cast<bool*>(data);
  *b = true;
}


void onLevels(InputLevels &levels) {
  rain.update(levels.rainFlow);
  toilet.update(levels.toiletFlow);
  washer.update(levels.washerFlow);
  dishes.update(levels.dishWasherFlow);
  shower.update(levels.showerFlow);
  updateEventsFlag = true;
}


void setup() {
  controllerSetup(6);
  extraProcessInputLevels = onLevels;
}

void loop() {
<<<<<<< HEAD
  cso.timeout.update();
  poonami.timeout.update();
  if (cso.state == 0 && cso.drainable && poonami.drainable) {
    mainDrain->setDraining();
  }

=======
  rain.timeout.update();
  toilet.timeout.update();
  washer.timeout.update();
  dishes.timeout.update();
  shower.timeout.update();

  if (updateEventsFlag) {
    updateEvents(
      rain.value, 
      toilet.value + washer.value + dishes.value + shower.value
    );
  }

  cso.timeout.update();
  poonami.timeout.update();
  if (cso.state == 0 && cso.drainable && poonami.drainable) {
    mainDrain->setDraining();
  }

>>>>>>> 2a9a4f0f9ac9e4a61653624d50e51a11889bb6b4
  if (poonami.state == 0 && poonami.drainable) {
    overflowDrain->setDraining();
  } else if (!poonami.drainable && overflowDrain->getOutputCount() > 0) {
    overflowDrain->setOverflowing();
  }
  controllerLoop(false);
}


