#include <stdlib.h>
#include <OctoWS2811.h>
#include "messages.h"
#include "flow.h"


// the leds
#define STRIP_LEN 280
byte pins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };
DMAMEM int displayMemory[STRIP_LEN*6];
OctoWS2811 strip(STRIP_LEN, displayMemory, nullptr, WS2811_RGB | WS2811_800kHz, 8, pins);


// globals for storing incoming CAN data
// InputLevels levels, nextLevels;
InputLevels levels;
InputLevels nextLevels = { 1, 1, 1, 1, 1 };
bool updatedLevels = false;


int invert(int x) {
  return x == 1 ? 0 : 1;
}

void tryUpdateLevels() {
  static unsigned long time = 1000;
  static unsigned int index = 0;
  if (millis() > time) {
    updatedLevels = true;
    time = millis() + 1000 + (rand() % 4000);
    // time = millis() + 2000;
    switch (index) {
      case 0:
        nextLevels.toiletFlow = invert(nextLevels.toiletFlow);
        break;
      case 1:
        nextLevels.showerFlow = invert(nextLevels.showerFlow);
        break;
      case 2:
        nextLevels.dishWasherFlow = invert(nextLevels.dishWasherFlow);
        break;
      case 3:
        nextLevels.washerFlow = invert(nextLevels.washerFlow);
        break;
      default:
        break;
    };
    index = rand() % 4;
    // index = 2;
  }
}


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
void startFlow(PipeSource *source) {
  for (; source != nullptr; source = source->next) {
    source->pipe->startFlow();
  }
}
void endFlow(PipeSource *source) {
  for (; source != nullptr; source = source->next) {
    source->pipe->endFlow();
  }
}


void blink(unsigned long time) {
  memset(displayMemory, 255, sizeof(displayMemory));
  strip.show();
  delay(time);
  memset(displayMemory, 0, sizeof(displayMemory));
  strip.show();
  delay(time);
}


void setup() {
  srand(0);
  Serial.begin(15200);
  delay(1000);
  setupCan();
  Serial.println("[pipes] boot!");
  strip.begin();
  blink(200); blink(200); blink(200);
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

    Serial.println("================================================================");
    Serial.print("rain: "); Serial.println(levels.rainFlow);
    Serial.print("toilet: "); Serial.println(levels.toiletFlow);
    Serial.print("washer: "); Serial.println(levels.washerFlow);
    Serial.print("dishwasher: "); Serial.println(levels.dishWasherFlow);
    Serial.print("shower: "); Serial.println(levels.showerFlow);
    Serial.println("\n");

    // update rain flows
    if (levels.rainFlow > 0) {
      startFlow(rains);
    } else {
      endFlow(rains);
    }


    // update toilet flows
    if (levels.toiletFlow > 0) {
      startFlow(toilets);
    } else {
      endFlow(toilets);
    }

    // update washer flows
    if (levels.washerFlow > 0) {
      startFlow(washers);
    } else {
      endFlow(washers);
    }

    // update dishwasher flows
    if (levels.dishWasherFlow > 0) {
      startFlow(dishwashers);
    } else {
      endFlow(dishwashers);
    }

    // update shower flows
    if (levels.showerFlow > 0) {
      startFlow(showers);
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
