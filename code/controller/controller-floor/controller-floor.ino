#include "flow.h"
#include "controller.h"
#include "timeout.h"


// #define MAIN_OUTPUT_MAX 230
// #define OVERFLOW_OUTPUT_MAX 330

// #define CSO_TIME 20000
// #define POONAMI_TIME 20000

// #define CSO_RAIN 2
// #define CSO_APPLIANCE 6
// #define POONAMI_RAIN 3
// #define POONAMI_APPLIANCE  9

// struct EventState {
//   uint8_t state = 0;
//   bool drainable = true;
//   Timeout timeout;
//   void updateState(uint8_t raw) {
//     state = ((state << 1) | raw) & 0x3;
//   }
// } cso, poonami;


// void setTrue(void *data) {
//   bool *b = static_cast<bool*>(data);
//   *b = true;
// }


// void onLevels(InputLevels &levels) {
//   unsigned int appliances =
//     levels.toiletFlow + levels.washerFlow +
//     levels.dishWasherFlow + levels.showerFlow;

//   cso.updateState(
//     levels.rainFlow >= CSO_RAIN && appliances >= CSO_APPLIANCE
//   );
//   if (cso.state == 1) {
//     cso.drainable = false;
//     mainDrain->setOverflowing();
//     cso.timeout.set(CSO_TIME, setTrue, &(cso.drainable));
//   }

//   poonami.updateState(
//     levels.rainFlow >= POONAMI_RAIN && appliances >= POONAMI_APPLIANCE
//   );
//   if (poonami.state == 1) {
//     poonami.drainable = false;
//     poonami.timeout.set(POONAMI_TIME, setTrue, &(poonami.drainable));
//   }

//   // if (levels.rainFlow >= POONAMI_RAIN && appliances >= POONAMI_APPLIANCE) {
// }


void setup() {
  controllerSetup(6);
  //extraProcessInputLevels = onLevels;
}

void loop() {
  // cso.timeout.update();
  // poonami.timeout.update();
  // onLevels();
  // if (cso.state == 0 && cso.drainable && poonami.drainable) {
  //   mainDrain->setDraining();
  // }

  // if (poonami.state == 0 && poonami.drainable) {
  //   overflowDrain->setDraining();
  // } else if (!poonami.drainable && overflowDrain->getOutputCount() > 0) {
  //   overflowDrain->setOverflowing();
  // }
  controllerLoop(false);
}
