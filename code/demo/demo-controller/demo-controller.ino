#include <OctoWS2811.h>
#include "messages.h"
#include "flow.h"


// the leds
#define STRIP_LEN 300
DMAMEM int displayMemory[STRIP_LEN*6];
OctoWS2811 strip(STRIP_LEN, displayMemory, nullptr, WS2811_RGB | WS2811_800kHz);


// globals for storing incoming CAN data
InputLevels levels, nextLevels;
bool updatedLevels = false;


// process an incoming InputLevels CAN msg
void processInputLevels(uint8_t src, InputLevels newLevels) {
  Serial.println("new levels!");
  memcpy(&nextLevels, &newLevels, sizeof(InputLevels));
  updatedLevels = true;
}


// pipe lists
// allPipes contains every pipe; the rest contain only specific ones
PipeSource *allPipes = nullptr;
PipeSource *rains = nullptr;
PipeSource *toilets = nullptr;
PipeSource *washers = nullptr;
PipeSource *dishwashers = nullptr;
PipeSource *showers = nullptr;


// helper functions to manage creating & removing flows
void startFlow(PipeSource *source, FlowType type) {
  for (; source != nullptr; source = source->next) {
    source->pipe->startFlow(type, 1);
  }
}
void endFlow(PipeSource *source) {
  for (; source != nullptr; source = source->next) {
    source->pipe->endFlow();
  }
}


void setup() {
  Serial.begin(15200);
  delay(200);
  setupCan();
  Serial.println("boot!");
  strip.begin();
  memset(displayMemory, 0, sizeof(displayMemory));
  strip.show();
  createPipes(
    strip, 
    &allPipes, &rains, 
    &toilets, &washers, 
    &dishwashers, &showers
  );
}


void loop() {
  // update flows if a CAN msg was received
  if (updatedLevels) {
    updatedLevels = false; // reset flag
    memcpy(&levels, &nextLevels, sizeof(InputLevels)); // copy levels

    // update rain flows
    if (levels.rainFlow > 0) {
      startFlow(rains, RAIN);
    } else {
      endFlow(rains);
    }


    // update toilet flows
    if (levels.toiletFlow > 0) {
      startFlow(toilets, TOILET);
    } else {
      endFlow(toilets);
    }

    // update washer flows
    if (levels.washerFlow > 0) {
      startFlow(washers, WASHER);
    } else {
      endFlow(washers);
    }

    // update dishwasher flows
    if (levels.dishWasherFlow > 0) {
      startFlow(dishwashers, DISHWASHER);
    } else {
      endFlow(dishwashers);
    }

    // update shower flows
    if (levels.showerFlow > 0) {
      startFlow(showers, SHOWER);
    } else {
      endFlow(showers);
    }
  }

  // wait for update to finish
  while (strip.busy()) {}

  // update & render all pipes
  for (PipeSource *source = allPipes; source != nullptr; source = source->next) {
    source->pipe->update();
    source->pipe->render();
  }

  // display
  strip.show();
}//*/
