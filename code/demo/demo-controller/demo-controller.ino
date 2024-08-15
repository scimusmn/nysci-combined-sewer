#include <Adafruit_NeoPixel.h>
#include "messages.h"
#include "flow.h"


// the leds
#define N_LEDS 395
#define LED_PIN 6
Adafruit_NeoPixel strip(N_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);


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
PipeSource *allPipes;
PipeSource *toilets;
PipeSource *washers;
PipeSource *dishwashers;
PipeSource *showers;


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
  strip.clear();
  createPipes(strip, &allPipes, &toilets, &washers, &dishwashers, &showers);
}


void loop() {
  // update flows if a CAN msg was received
  if (updatedLevels) {
    updatedLevels = false; // reset flag
    memcpy(&levels, &nextLevels, sizeof(InputLevels)); // copy levels

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

  // update & render all pipes
  for (PipeSource *source = allPipes; source != nullptr; source = source->next) {
    source->pipe->update();
    source->pipe->render();
  }

  // display
  strip.show();
  delay(20);
}//*/
