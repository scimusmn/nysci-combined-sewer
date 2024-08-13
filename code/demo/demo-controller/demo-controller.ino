#include <Adafruit_NeoPixel.h>
#include "messages.h"
#include "flow.h"


#define N_LEDS 395
#define LED_PIN 6
Adafruit_NeoPixel strip(N_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);


InputLevels levels;
bool flag = false;

void processInputLevels(uint8_t src, InputLevels newLevels) {
  memcpy(&levels, &newLevels, sizeof(InputLevels));
  flag = true;
}


PipeSource *allPipes;
PipeSource *toilets;
PipeSource *washers;
PipeSource *dishwashers;
PipeSource *showers;


void setup() {
  Serial.begin(15200);
  delay(200);
  // setupCan();
  Serial.println("boot!");
  strip.begin();
  strip.clear();

  PipeSource *list[4];
  createPipes(list, strip);
  allPipes    = list[0];
  toilets     = list[1];
  washers     = list[2];
  dishwashers = list[3];
  showers     = list[4];
}


// const int x0 = 100;
// const int x1 = 111-100;
// void loop() {
//   static int k = 0;
//   strip.fill(0x112288, x0, x1);
//   strip.show();
// }


void loop() {
  static unsigned long k = 0;
  strip.fill(0);
  k += 1;
  if (k % 40 == 0) {
    for (PipeSource *source = showers; source != nullptr; source = source->next) {
      source->pipe->startFlow(FlowType::SHOWER, 1);
    }
  } else if (k % 40 == 7) {
    for (PipeSource *source = showers; source != nullptr; source = source->next) {
      source->pipe->endFlow();
    }
  }

  if (k % 157 == 0) {
    for (PipeSource *source = toilets; source != nullptr; source = source->next) {
      source->pipe->startFlow(FlowType::TOILET, 1);
    }
  } else if (k % 157 == 7) {
    for (PipeSource *source = toilets; source != nullptr; source = source->next) {
      source->pipe->endFlow();
    }
  }

  if (k % 299 == 0) {
    for (PipeSource *source = dishwashers; source != nullptr; source = source->next) {
      source->pipe->startFlow(FlowType::DISHWASHER, 1);
    }
  } else if (k % 299 == 7) {
    for (PipeSource *source = dishwashers; source != nullptr; source = source->next) {
      source->pipe->endFlow();
    }
  }

  if (k % 333 == 0) {
    for (PipeSource *source = washers; source != nullptr; source = source->next) {
      source->pipe->startFlow(FlowType::WASHER, 1);
    }
  } else if (k % 333 == 7) {
    for (PipeSource *source = washers; source != nullptr; source = source->next) {
      source->pipe->endFlow();
    }
  }



  for (PipeSource *source = allPipes; source != nullptr; source = source->next) {
    source->pipe->update();
    source->pipe->render();
  }
  strip.show();
  delay(20);
}
