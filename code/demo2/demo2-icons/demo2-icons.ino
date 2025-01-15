#include <OctoWS2811.h>
#include "messages.h"

InputLevels levels;
InputLevels nextLevels = { 1, 1, 1, 1, 1 };
//InputLevels nextLevels = { 0, 0, 0, 0, 0 };
bool updatedLevels = true;


void processInputLevels(uint8_t src, InputLevels levels) {
  //Serial.println("new levels!");
  memcpy(&nextLevels, &levels, sizeof(InputLevels));
  updatedLevels = true;
}



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




// the leds
typedef OctoWS2811 LedStrip_t;
#define LEDS_PER_STRIP 420
#define NUM_PINS 8
uint8_t ledPins[] = { 2, 3, 4, 5, 6, 7, 8, 9 };
DMAMEM int displayMemory[LEDS_PER_STRIP * NUM_PINS * 3 / 4];
LedStrip_t strip(LEDS_PER_STRIP, displayMemory, nullptr, WS2811_RGB | WS2811_800kHz, NUM_PINS, ledPins);


class Icon;

struct IconList {
  Icon *icon = nullptr;
  IconList *next = nullptr;
};

// icon class
// #define ICON_COLOR 0xff, 0xff, 0xff
#define ICON_COLOR 0xffffff
class Icon {
  public:
  Icon(size_t begin, size_t end, IconList *list=nullptr) 
    : begin(begin), end(end) {
    if (list != nullptr) {
      IconList *self = new IconList;
      self->icon = this;
      self->next = list->next;
      list->next = self;
      Serial.println((unsigned int) list->next->icon, HEX);
      Serial.println((unsigned int) this, HEX);
    }
  }
  void show() {
    for (int i=begin; i<end; i++) {
      strip.setPixel(i, ICON_COLOR);
    }
  }
  void hide() {
    for (int i=begin; i<end; i++) {
      strip.setPixel(i, 0);
    }
  }

  // protected:
  size_t begin, end;
};


IconList *toilet = new IconList;
IconList *shower = new IconList;
IconList *washer = new IconList;
IconList *dishwasher = new IconList;


void setupIcons();
void updateIconList(IconList *list, int level);
int levelMap(int level, int count);
void updateIcons();




void setupIcons() {
  toilet->next = nullptr;
  shower->next = nullptr;
  washer->next = nullptr;
  dishwasher->next = nullptr;
  // cr
  Icon *cr0 = new Icon(0, 58, dishwasher);
  Icon *cr1 = new Icon(59, 116, shower);
  Icon *cr2 = new Icon(117, 174, toilet);
  Icon *cr4 = new Icon(420, 480, toilet);
  Icon *cr5 = new Icon(481, 538, shower);
  Icon *cr6 = new Icon(539, 596, dishwasher);
  Icon *cr7 = new Icon(597, 655, dishwasher);

  // fr
  Icon *fr0 = new Icon(656, 713, shower);
  Icon *fr1 = new Icon(714, 840, toilet);
  Icon *fr2 = new Icon(841, 900, dishwasher);
  Icon *fr3 = new Icon(900, 960, washer);
  Icon *fr4 = new Icon(960, 1020, shower);
  Icon *fr5 = new Icon(1020, 1080, toilet);
  Icon *fr6 = new Icon(1080, 1140, toilet);
  Icon *fr7 = new Icon(1140, 1200, toilet);


  // cr
  // Icon *cr0 = new Icon(1740, 1800, shower);
  // Icon *cr1 = new Icon(1680, 1740, toilet);
  // Icon *cr2 = new Icon(1860, 1920, washer);
  // Icon *cr3 = new Icon(1800, 1860, dishwasher);
  // Icon *cr4 = new Icon(2160, 2220, shower);
  // Icon *cr5 = new Icon(2100, 2160, toilet);
  // Icon *cr6 = new Icon(2280, 2340, washer);
  // Icon *cr7 = new Icon(2220, 2280, dishwasher);

  // // fr
  // Icon *fr0 = new Icon(1020, 1080, washer);
  // Icon *fr1 = new Icon(960, 1020, dishwasher);
  // Icon *fr2 = new Icon(900, 960, shower);
  // Icon *fr3 = new Icon(840, 900, toilet);
  // Icon *fr4 = new Icon(1619, 1678, washer);
  // Icon *fr5 = new Icon(1559, 1618, dishwasher);
  // Icon *fr6 = new Icon(1499, 1558, shower);
  // Icon *fr7 = new Icon(1440, 1499, toilet);
  // Icon *fr8 = new Icon(1381, 1439, dishwasher);
  // Icon *fr9 = new Icon(1321, 1380, dishwasher);
  // Icon *fr10 = new Icon(1260, 1320, toilet);
}


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

void updateIconList(IconList *list, int level) {
  for (IconList *it = list->next; it != nullptr; it = it->next) {
    Icon *icon = it->icon;
    if (level > 0) {
      icon->show();
      Serial.print("show "); Serial.print(icon->begin); Serial.print(", "); Serial.println(icon->end);
    } else {
      icon->hide();
    }
  }
}

void updateIcons() {
  updateIconList(toilet, levels.toiletFlow);
  updateIconList(washer, levels.washerFlow);
  updateIconList(dishwasher, levels.dishWasherFlow);
  updateIconList(shower, levels.showerFlow);
  strip.show();
}



// main setup/loop
void setup() {
  srand(0);
  Serial.begin(9600);
  delay(1000);
  Serial.println("[icons] boot!");
  setupIcons();
  setupCan();
  strip.begin();
  Serial.println("setup complete");
  // updateIcons();
}


void loop() {
  // Serial.println("toilet");
  // updateIconList(toilet, 1);
  // strip.show();
  // delay(1000);
  // updateIconList(toilet, 0);
  // Serial.println("shower");
  // updateIconList(shower, 1);
  // strip.show();
  // delay(1000);
  // updateIconList(shower, 0);
  // Serial.println("washer");
  // updateIconList(washer, 1);
  // strip.show();
  // delay(1000);
  // updateIconList(washer, 0);
  // Serial.println("dishwasher");
  // updateIconList(dishwasher, 1);
  // strip.show();
  // delay(1000);
  // updateIconList(dishwasher, 0);
  //tryUpdateLevels();
  if (updatedLevels) {
    Serial.println("UPDATED LEVELS");
    updatedLevels = false; // reset flag
    memcpy(&levels, &nextLevels, sizeof(InputLevels)); // copy levels
    updateIcons();
  }
}
