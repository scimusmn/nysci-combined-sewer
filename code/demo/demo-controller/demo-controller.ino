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
  // setupCan();
  Serial.println("boot!");
  strip.begin();
  strip.clear();
  createPipes(strip, &allPipes, &toilets, &washers, &dishwashers, &showers);
}


/*const int x0 = 273;
const int x1 = 286;
//const int x1 = 292;
void loop() {
  static int k = 0;
  strip.fill(0x112288, x0, x1-x0);
  strip.show();
}//*/


#define INTERMITTENT(SOURCES, TYPE, FREQ, LEN) \
  do { \
    if (k % FREQ == 0) { \
      startFlow(SOURCES, TYPE); \
    } else if (k % FREQ == LEN) { \
      endFlow(SOURCES); \
    } \
  } while (0)


void loop() {
  static unsigned long k = 0;
  strip.fill(0);
  k += 1;
  INTERMITTENT(toilets, TOILET, 10, 3);
  INTERMITTENT(showers, SHOWER, 10, 5);
  INTERMITTENT(washers, WASHER, 42, 4);
  INTERMITTENT(dishwashers, DISHWASHER, 59, 6);

  for (PipeSource *source = allPipes; source != nullptr; source = source->next) {
    source->pipe->update();
    source->pipe->render();
  }
  strip.show();
  delay(200);
}//*/
