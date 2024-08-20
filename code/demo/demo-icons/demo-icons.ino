#include <Adafruit_NeoPixel.h>
#include "messages.h"


// process incoming CAN messages
volatile InputLevels levels;
volatile bool flag = false;
void processInputLevels(uint8_t src, InputLevels update) {
  memcpy(&levels, &update, sizeof(InputLevels));
  flag = true;
}


// the leds
typedef Adafruit_NeoPixel LedStrip_t;
#define LED_PIN    8
#define LED_COUNT (11*60)
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


// icon class
#define ICON_COLOR 0xff, 0xff, 0xff
class Icon {
  public:
  Icon(LedStrip_t &strip, size_t len) 
    : strip(strip), start(Icon::START), len(len) {
    Icon::START += len;
  }
  void show() {
    for (int i=0; i<len; i++) {
      strip.setPixelColor(i+start, ICON_COLOR);
    }
  }
  void hide() {
    for (int i=0; i<len; i++) {
      strip.setPixelColor(i+start, 0);
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
  levels = { 0, 0, 0, 0, 0 };
  updateIcons();
}


void loop() {
  if (flag) {
    flag = false;
    updateIcons();
  }
}
