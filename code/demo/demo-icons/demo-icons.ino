#include <OctoWS2811.h>
#include "messages.h"


// process incoming CAN messages
volatile InputLevels levels;
volatile bool flag = false;
void processInputLevels(uint8_t src, InputLevels update) {
  memcpy(&levels, &update, sizeof(InputLevels));
  flag = true;
}


// the leds
typedef OctoWS2811 LedStrip_t;
#define LEDS_PER_STRIP 60*4
#define NUM_PINS 3
uint8_t ledPins[] = { 8, 7, 13 };
DMAMEM int displayMemory[LEDS_PER_STRIP * NUM_PINS * 3 / 4];
LedStrip_t strip(LEDS_PER_STRIP, displayMemory, nullptr, WS2811_RGB | WS2811_800kHz, NUM_PINS, ledPins);


// icon class
// #define ICON_COLOR 0xff, 0xff, 0xff
#define ICON_COLOR 0xffffff
class Icon {
  public:
  Icon(LedStrip_t &strip, size_t len) 
    : strip(strip), start(Icon::START), len(len) {
    Icon::START += len;
  }
  void show() {
    for (int i=0; i<len; i++) {
      strip.setPixel(i+start, ICON_COLOR);
    }
  }
  void hide() {
    for (int i=0; i<len; i++) {
      strip.setPixel(i+start, 0);
    }
  }
  static size_t START;

  protected:
  LedStrip_t &strip;
  size_t start, len;
};


size_t Icon::START = 0;


// icon instances
Icon dishwasherLM0(strip, 61);
Icon dishwasherLM1(strip, 60);
Icon toiletLM(strip,      60);
Icon dummy(strip, 59);

Icon washerUM(strip,      60);
Icon dishwasherUM(strip,  60);
Icon showerUM(strip,      60);
Icon toiletUM(strip,      60);

Icon washerTop(strip,     60);
Icon dishwasherTop(strip, 60);
Icon showerTop(strip,     60);
Icon toiletTop(strip,     60);


int levelMap(int level, int count) {
  if (level) {
    return count;
  } else {
    return 0;
  }
  /*if (level == 2) {
    return min(2, count);
  } else if (level == 3) {
    return count;
  } else {
    return level;
  }*/
}

void updateIconList(Icon **list, int level, int count);
void updateIconList(Icon **list, int level, int count) {
  for (int i=0; i<count; i++) {
    Icon *icon = list[i];
    if (levelMap(level, count) > i) {
      icon->show();
    } else {
      icon->hide();
    }
  }
}

void updateIcons() {
  Icon* toilets[] = { &toiletTop, &toiletUM, &toiletLM };
  const int numToilets = sizeof(toilets) / sizeof(Icon*);
  updateIconList(toilets, levels.toiletFlow, numToilets);

  Icon* washers[] = { &washerTop, &washerUM };
  const int numWashers = sizeof(washers) / sizeof(Icon*);
  updateIconList(washers, levels.washerFlow, numWashers);

  Icon* dishwashers[] = { 
    &dishwasherTop, &dishwasherUM,
    &dishwasherLM0, &dishwasherLM1,
  };
  const int numDishwashers = sizeof(dishwashers) / sizeof(Icon*);
  updateIconList(dishwashers, levels.dishWasherFlow, numDishwashers);

  Icon* showers[] = { &showerTop, &showerUM };
  const int numShowers = sizeof(showers) / sizeof(Icon*);
  updateIconList(showers, levels.showerFlow, numShowers);

  strip.show();
}



// main setup/loop
void setup() {
  setupCan();
  strip.begin();
  levels = { 1, 1, 1, 1, 1 };
  updateIcons();
}


void loop() {
  if (flag) {
    flag = false;
    updateIcons();
  }
}
