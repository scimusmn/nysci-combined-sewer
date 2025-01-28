#include <stdlib.h>
#include <OctoWS2811.h>
#include "messages.h"
#include "flow.h"


// the leds
#define STRIP_LEN 290
byte pins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };
DMAMEM int displayMemory[STRIP_LEN*6];
OctoWS2811 strip(STRIP_LEN, displayMemory, nullptr, WS2811_RGB | WS2811_800kHz, 8, pins);


// globals for storing incoming CAN data
// InputLevels levels, nextLevels;
InputLevels levels;
InputLevels nextLevels = { 0, 0, 0, 0, 0 };
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
  }
}

// pipe lists
PipeCollections pipes;

void toggleOverflow() {
  static unsigned long time = 0;
  static bool overflowing = true;
  if (millis() > time) {
    Serial.print("overflow: "); Serial.println(overflowing);
    time = millis() + 30000;
    if (overflowing) {
      pipes.pipes->next->next->pipe->setOverflowing();
    } else {
      pipes.pipes->next->next->pipe->setDraining();
    }
    overflowing = !overflowing;
  }
}

// process an incoming InputLevels CAN msg
void processInputLevels(uint8_t src, InputLevels newLevels) {
  // Serial.println("new levels!");
  memcpy(&nextLevels, &newLevels, sizeof(InputLevels));
  updatedLevels = true;
}

void processPipeOutput(uint8_t src, CanPipeOutput output) {
  // Serial.print("rx output! node: "); Serial.print(src);
  // Serial.print(", pipe: "); Serial.print(output.pipeId);
  // Serial.print(", count: "); Serial.println(output.count);
  for (PipeSource *source = pipes.pipes; source != nullptr; source = source->next) {
    source->pipe->updateCanInput(src, output);
  }
}

void processPipeOverflow(uint8_t src, CanPipeOverflow o) {
  Serial.print("rx pipe overflow "); Serial.print(o.node); Serial.print(":"); Serial.println(o.pipeId);
  for (PipeSource *source = pipes.pipes; source != nullptr; source = source->next) {
    source->pipe->updateCanOverflow(src, o);
  }
}

// helper functions to manage creating & removing flows
void startFlow(PipeSource *source, unsigned int level=0) {
  for (; source != nullptr; source = source->next) {
    source->pipe->startFlow(1, level);
  }
}
void endFlow(PipeSource *source) {
  for (; source != nullptr; source = source->next) {
    source->pipe->endFlow();
  }
}


void controllerSetup(uint8_t canBusId) {
  srand(0);
  Serial.begin(15200);
  delay(1000);
  setupCan(canBusId);
  Serial.println("[pipes] boot!");
  strip.begin();
  memset(displayMemory, 0, sizeof(displayMemory));
  strip.show();
  createPipes(strip, &pipes);
  startFlow(pipes.constant);
}


void controllerLoop(bool debug=false) {
  if (debug) {
    tryUpdateLevels();
    toggleOverflow();
  }
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

    startFlow(pipes.rains, levels.rainFlow);
    startFlow(pipes.toilets, levels.toiletFlow);
    startFlow(pipes.washers, levels.washerFlow);
    startFlow(pipes.dishwashers, levels.dishWasherFlow);
    startFlow(pipes.showers, levels.showerFlow);
  }

  // wait for update to finish
  while (strip.busy()) {}

  // update & render all pipes
  for (PipeSource *source = pipes.pipes; source != nullptr; source = source->next) {
    source->pipe->update();
    source->pipe->render();
  }

  // display
  strip.show();
}//*/
