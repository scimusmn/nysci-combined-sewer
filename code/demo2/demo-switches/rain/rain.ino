#include <OctoWS2811.h>
#include "messages.h"

// globals for storing incoming CAN data
InputLevels levels, nextLevels;
bool updatedLevels = false;

// process an incoming InputLevels CAN msg
void processInputLevels(uint8_t src, InputLevels newLevels) {
  Serial.println("new levels!");
  memcpy(&nextLevels, &newLevels, sizeof(InputLevels));
  updatedLevels = true;
}


#define NUM_GROUPS 4
#define STRIP_LENGTH 132
#define RAINDROP_SIZE 8
#define NUM_RAINDROPS 4
#define DROP_COUNT_SLEW_TIME 1000
#define DROP_ADVANCE_INTERVAL 4



byte pinList[NUM_GROUPS] = {2,3,4,5};
DMAMEM int displayMemory[STRIP_LENGTH*6];
// int drawingMemory[STRIP_LENGTH*6];
OctoWS2811 leds(STRIP_LENGTH, displayMemory, nullptr, WS2811_GRB | WS2811_800kHz, NUM_GROUPS, pinList);


struct RainDrop {
  int position = 0;
  int start;
  bool active = false;
  int speed;

  // advance + render the drop
  void advance(OctoWS2811 &leds) {
    if(position == STRIP_LENGTH+RAINDROP_SIZE){
      // ran off the end
      position = 0;
      active = false;
    } else {
      // render drop
      for (int x=0; x<RAINDROP_SIZE; x++) {
        int pixel = position - x;
        if(pixel >= 0 && pixel < STRIP_LENGTH) {
          leds.setPixel(pixel+start, color[x]/10, color[x]/10, color[x]);
        }
      }
      // advance one step
      position += speed;
    }
  }

  // shared drop colors
  static int color[RAINDROP_SIZE];
  static void setupColor() {
    for(int x = 0; x < RAINDROP_SIZE; x++){
      color[x] = 255 * exp(-0.5*x); 
    }
    color[RAINDROP_SIZE - 3] = 0;
    color[RAINDROP_SIZE - 2] = 0;
    color[RAINDROP_SIZE - 1] = 0;
  }
};
int RainDrop::color[RAINDROP_SIZE];



class RainGroup {
  public:
  // (constructor)
  RainGroup(OctoWS2811 &strip, unsigned int group)
  : strip(strip), group(group) {
    for (int i=0; i<NUM_RAINDROPS; i++) {
      raindrops[i].start = STRIP_LENGTH * group;
      raindrops[i].speed = random(2, 4);
    }
  }

  // add a new active drop if it is time to trigger
  void tryAddNewDrop() {
    if (millis() > triggerTime) {
      triggerTime = millis() + (10 * random(5, 50));
      for (int i=0; i < currentDropCount; i++) {
        if (!raindrops[i].active) {
          raindrops[i].active = true;
          break;
        }
      }
    }
  }

  // advance + render active drops
  void advance() {
    for (int i=0; i<NUM_RAINDROPS; i++) {
      if (raindrops[i].active) {
        raindrops[i].advance(strip);
      }
    }
  }

  static unsigned int targetDropCount;
  static unsigned int currentDropCount;

  private:
  OctoWS2811 &strip;
  unsigned int group;
  RainDrop raindrops[NUM_RAINDROPS];
  unsigned long triggerTime = 0;
};
unsigned int RainGroup::targetDropCount = NUM_RAINDROPS;
unsigned int RainGroup::currentDropCount = 0;


RainGroup rain[NUM_GROUPS] = {
  RainGroup(leds, 0),
  RainGroup(leds, 1),
  RainGroup(leds, 2),
  RainGroup(leds, 3),
};


// update rain animation
void updateRain() {
  static unsigned long stepCountTime = 0;
  static unsigned long advanceTime = 0;

  // slew the current drop count?
  if (millis() > stepCountTime) {
    // move current drop count towards the target
    stepCountTime = millis() + DROP_COUNT_SLEW_TIME;
    if (RainGroup::currentDropCount < RainGroup::targetDropCount) {
      RainGroup::currentDropCount += 1;
    } else if (RainGroup::currentDropCount > RainGroup::targetDropCount) {
      RainGroup::targetDropCount -= 1;
    }
  }

  // insert new drops if needed
  for (int i=0; i<NUM_GROUPS; i++) {
    rain[i].tryAddNewDrop();
  }

  // advance existing drops?
  if (millis() > advanceTime) {
    advanceTime = millis() + DROP_ADVANCE_INTERVAL;
    for (int i=0; i<NUM_GROUPS; i++) {
      rain[i].advance();
    }
  }
}


// setup stuff
void setup() {
  leds.begin();
  leds.show();
  setupCan();
  Serial.begin(9600);
  delay(1000);
  Serial.println("start!!");
  RainDrop::setupColor();
}


// loop!!!!
void loop() {
  updateRain();
  leds.show();
}
