#define SMM_IMPLEMENTATION
#include "smm.h"
#include "messages.h"
#include "rain.h"

#define LEVEL_TIMEOUT 3000

// #define RAIN_PIN 16
// #define TOILET_PIN 17
#define TOILET_PIN 17
#define RAIN_PIN 16

#define WASHER_PIN 14
#define DISHWASHER_PIN 18
#define SHOWER_PIN 15


InputLevels levels = {
  0, 1, 2, 3, 4,
};

class LevelSwitch : public smm::Switch {
  public:
  LevelSwitch(int pin, uint8_t *level) 
  : smm::Switch(pin), level(level)
  {}
  void onLow() {
    active = true;
    *level = min(max(*level+1,holdLevel+1), 3);
    holdLevel = *level;
    updateFlag = true;
    if (time == 0) {
      time = millis() + LEVEL_TIMEOUT;
    }
  }
  void onHigh() {
    active = false;
    *level = 0;
    //if(holdLevel == 3){holdLevel = 0;}
    updateFlag = true;
    // if (millis() > time) {
    //   updateFlag = true;
    //   time = 0;
    // }
  }
  void update() {
    if (!active && time != 0 && millis() > time) {
      holdLevel = 0;
      *level = 0;
      updateFlag = true;
      time = 0;
    }
  }
  static volatile bool updateFlag;
  private:
  uint8_t *level;
  uint8_t holdLevel;
  bool active = false;
  unsigned long time = 0;
};
volatile bool LevelSwitch::updateFlag = false;

LevelSwitch rainSwitch(RAIN_PIN, &(levels.rainFlow));
LevelSwitch toiletSwitch(TOILET_PIN, &(levels.toiletFlow));
LevelSwitch washerSwitch(WASHER_PIN, &(levels.washerFlow));
LevelSwitch dishSwitch(DISHWASHER_PIN, &(levels.dishWasherFlow));
LevelSwitch showerSwitch(SHOWER_PIN, &(levels.showerFlow));

void updateSwitches() {
  rainSwitch.update();
  toiletSwitch.update();
  washerSwitch.update();
  dishSwitch.update();
  showerSwitch.update();
}


void processInputLevels(uint8_t src, InputLevels levels) {}


void setup() {
  Serial.begin(9600);
  delay(1000);
  setupCan();
  Serial.println("[switches] boot!");
  smm::setup();
  setupRain();
}


void loop() {
  updateRain(levels.rainFlow);
  updateSwitches();
  // Serial.println("[switches] update!");
  if (LevelSwitch::updateFlag) {
    LevelSwitch::updateFlag = false;
    sendInputLevels(0, levels);
    Serial.println("================================================================");
    Serial.print("rain: "); Serial.println(levels.rainFlow);
    Serial.print("toilet: "); Serial.println(levels.toiletFlow);
    Serial.print("washer: "); Serial.println(levels.washerFlow);
    Serial.print("dishwasher: "); Serial.println(levels.dishWasherFlow);
    Serial.print("shower: "); Serial.println(levels.showerFlow);
    Serial.println("\n");
  }
}
