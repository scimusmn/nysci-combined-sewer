#include <OctoWS2811.h>
#include "messages.h"

InputLevels levels;
//InputLevels nextLevels = { 1, 1, 1, 1, 1 };
InputLevels nextLevels = { 0, 0, 0, 0, 0};
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
  void setLevel(unsigned int level);
  unsigned int timeout = 0;
  unsigned int level = 0;
  bool on;
  void update(unsigned int level) {
    if(level > 0 && this->level == 0){
      timeout = millis()+3000;
      setLevel(level);
    }
    else if(level > this->level){setLevel(level);}
    else if (level == 0 && this->level != level && millis() > timeout){setLevel(level);}
    // if (level == 0 && this->level > 0) {
    //   if (millis() > timeout) {
    //     setLevel(level);
    //   }
    // } else if (level != this->level) {
    //   if (level == 0) {
    //     timeout = millis() + 3000;
    //   }
    //   setLevel(level);
    // }
  }
};

// icon class
// #define ICON_COLOR 0xff, 0xff, 0xff
#define ICON_COLOR 0xffffff
class Icon {
  public:
  unsigned int level;

  Icon(size_t begin, size_t end, int Level, IconList *list=nullptr) 
    : begin(begin), end(end) {
    if (list != nullptr) {
      IconList *self = new IconList;
      self->icon = this;
      self->next = list->next;
      list->next = self;
      level = Level;
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

void IconList::setLevel(unsigned int level) {
  this->level = level;
  for (IconList *it = next; it != nullptr; it = it->next) {
    Icon *icon = it->icon;
    if (level > 0) {
      if(icon->level <= level){icon->show();}
      Serial.print("show "); Serial.print(icon->begin); Serial.print(", "); Serial.println(icon->end);
    } else {
      icon->hide();
    }
  }
}


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
  // fl
  Icon *fl0 = new Icon(0, 58, 1, dishwasher);
  Icon *fl1 = new Icon(59, 116, 1, shower);
  Icon *fl2 = new Icon(117, 174, 1, toilet);
  Icon *fl3 = new Icon(420, 480, 3,toilet);
  Icon *fl4 = new Icon(481, 538, 2, shower);
  Icon *fl5 = new Icon(539, 596, 3, dishwasher);
  Icon *fl6 = new Icon(597, 655, 2, dishwasher);
  Icon *fl7 = new Icon(656, 713, 2, shower);
  Icon *fl8 = new Icon(714, 840, 2, toilet);
  Icon *fl9 = new Icon(841, 900, 1, dishwasher);
  Icon *fl10 = new Icon(900, 960, 1, washer);
  Icon *fl11 = new Icon(960, 1020, 3, shower);
  Icon *fl12 = new Icon(1020, 1080, 3, toilet);
  Icon *fl13 = new Icon(1080, 1140, 2, toilet);
  Icon *fl14 = new Icon(1140, 1200, 1, toilet);
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

void updateIcons() {
  toilet->update(levels.toiletFlow);
  washer->update(levels.washerFlow);
  dishwasher->update(levels.dishWasherFlow);
  shower->update(levels.showerFlow);
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
  updateIcons();
  if (updatedLevels) {
    Serial.println("UPDATED LEVELS");
    updatedLevels = false; // reset flag
    memcpy(&levels, &nextLevels, sizeof(InputLevels)); // copy levels
  }
}
