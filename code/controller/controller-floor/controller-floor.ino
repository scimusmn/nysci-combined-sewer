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


void setTrue(void *data) {
  bool *b = static_cast<bool*>(data);
  *b = true;
}


void onLevels(InputLevels &levels) {
  unsigned int appliances =
    levels.toiletFlow + levels.washerFlow +
    levels.dishWasherFlow + levels.showerFlow;

  cso.updateState(
    levels.rainFlow >= CSO_RAIN && appliances >= CSO_APPLIANCE
  );
  if (cso.state == 1) {
    cso.drainable = false;
    mainDrain->setOverflowing();
    cso.timeout.set(CSO_TIME, setTrue, &(cso.drainable));
  }

  poonami.updateState(
    levels.rainFlow >= POONAMI_RAIN && appliances >= POONAMI_APPLIANCE
  );
  if (poonami.state == 1) {
    poonami.drainable = false;
    poonami.timeout.set(POONAMI_TIME, setTrue, &(poonami.drainable));
  }

  // if (levels.rainFlow >= POONAMI_RAIN && appliances >= POONAMI_APPLIANCE) {
}



// void holdLevels(){
//   int Hold = 4000;
//   static unsigned long rainHold;
//   static unsigned long toiletHold;
//   static unsigned long washerHold;
//   static unsigned long showerHold;
//   static unsigned long dishwasherHold;

//   if(nextLevels.rainFlow < levels.rainFlow && rainHold == 0){rainHold = millis() + Hold;}
//   else if(nextLevels.rainFlow > levels.rainFlow){
//     levels.rainFlow = nextLevels.rainFlow;
//     updatedLevels = true;
//   }
//   else if(rainHold <= millis() && rainHold != 0){
//     rainHold = 0;
//     levels.rainFlow = 0;
//     updatedLevels = true;
//   }



//   if(nextLevels.toiletFlow < levels.toiletFlow && toiletHold == 0){toiletHold = millis() + Hold;}
//   else if(nextLevels.toiletFlow > levels.toiletFlow){
//     levels.toiletFlow = nextLevels.toiletFlow;
//     updatedLevels = true;
//   }
//   else if(toiletHold <= millis() && toiletHold != 0){
//     toiletHold = 0;
//     levels.toiletFlow = 0;
//     updatedLevels = true;
//   }



//   if(nextLevels.washerFlow < levels.washerFlow && washerHold == 0){washerHold = millis() + Hold;}
//   else if(nextLevels.washerFlow > levels.washerFlow){
//     levels.washerFlow = nextLevels.washerFlow;
//     updatedLevels = true;
//   }
//   else if(washerHold <= millis() && washerHold != 0){
//     washerHold = 0;
//     levels.washerFlow = 0;
//     updatedLevels = true;
//   }



//   if(nextLevels.showerFlow < levels.showerFlow && showerHold == 0){showerHold = millis() + Hold;}
//   else if(nextLevels.showerFlow > levels.showerFlow){
//     levels.showerFlow = nextLevels.showerFlow;
//     updatedLevels = true;
//   }
//   else if(showerHold <= millis() && showerHold != 0){
//     showerHold = 0;
//     levels.showerFlow = 0;
//     updatedLevels = true;
//   }



//   if(nextLevels.dishWasherFlow < levels.dishWasherFlow && dishwasherHold == 0){dishwasherHold = millis() + Hold;}
//   else if(nextLevels.dishWasherFlow > levels.dishWasherFlow){
//     levels.dishWasherFlow = nextLevels.dishWasherFlow;
//     updatedLevels = true;
//   }
//   else if(dishwasherHold <= millis() && dishwasherHold != 0){
//     dishwasherHold = 0;
//     levels.dishWasherFlow = 0;
//     updatedLevels = true;
//   }
// }




void setup() {
  controllerSetup(6);
  extraProcessInputLevels = onLevels;
}

void loop() {
  cso.timeout.update();
  poonami.timeout.update();
  if (cso.state == 0 && cso.drainable && poonami.drainable) {
    mainDrain->setDraining();
  }

  if (poonami.state == 0 && poonami.drainable) {
    overflowDrain->setDraining();
  } else if (!poonami.drainable && overflowDrain->getOutputCount() > 0) {
    overflowDrain->setOverflowing();
  }
  controllerLoop(false);
}


