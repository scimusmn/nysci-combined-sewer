#include "switches.h"
#include "messages.h"


void processInputLevels(uint8_t src, InputLevels levels) {}


InputLevels levels = {
  0, 1, 2, 3, 4,
};

void setup() {
  Serial.begin(115200);
  delay(200);
  setupCan();
  Serial.println("[switches] boot!");
  setupSwitches();
}


void loop() {
  bool update = false;
  #define X(k) do { \
    update = update || SWITCH(k).update; \
    SWITCH(k).update = false; \
    if (millis() > SWITCH(k).endTime && SWITCH(k).level > 0 && SWITCH(k).state == true) { \
      SWITCH(k).level = 0; \
      update = true; \
    } \
  } while (0);
  SWITCHES
  #undef X

  if (update) {
    InputLevels levels = {
      SWITCH(RAIN_PIN).level,
      SWITCH(TOILET_PIN).level,
      SWITCH(WASHER_PIN).level,
      SWITCH(DISHWASHER_PIN).level,
      SWITCH(SHOWER_PIN).level,
    };
    sendInputLevels(0, levels);
    Serial.println("================================================================");
    Serial.print("rain: "); Serial.println(SWITCH(RAIN_PIN).level);
    Serial.print("toilet: "); Serial.println(SWITCH(TOILET_PIN).level);
    Serial.print("washer: "); Serial.println(SWITCH(WASHER_PIN).level);
    Serial.print("dishwasher: "); Serial.println(SWITCH(DISHWASHER_PIN).level);
    Serial.print("shower: "); Serial.println(SWITCH(SHOWER_PIN).level);
    Serial.println("\n");
  }
}
