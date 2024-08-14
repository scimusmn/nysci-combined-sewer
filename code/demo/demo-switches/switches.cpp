#include <Arduino.h>
#include "switches.h"


#define PULL_TIME 500
#define MAX_LEVEL 3

#define ISR_NAME(X) PIN_ISR_ ## X

#define ISR(X) \
  volatile SwitchState SWITCH(X) = { true, 0, 0, 0, 0 }; \
  void ISR_NAME(X)() { \
    bool state = digitalRead(X); \
    if (state != SWITCH(X).state && millis() > SWITCH(X).debounce) { \
      /*Serial.print(#X); Serial.print(" "); Serial.println(state); */\
      SWITCH(X).state = state; \
      SWITCH(X).update = true; \
      SWITCH(X).debounce = millis() + 20; \
      \
      if (state == false) { \
        if (SWITCH(X).level == 0) { \
          SWITCH(X).level = 1; \
          SWITCH(X).endTime = millis() + PULL_TIME; \
        } else if (SWITCH(X).level < MAX_LEVEL) { \
          SWITCH(X).level += 1; \
        } \
      } else if (millis() > SWITCH(X).endTime) { \
        SWITCH(X).level = 0; \
      } \
      /*Serial.print("switch " #X ", level "); Serial.println(SWITCH(X).level); */\
    } \
  }

#define ATTACH_ISR(X) \
  do { \
    pinMode(X, INPUT_PULLUP); \
    attachInterrupt(digitalPinToInterrupt(X), PIN_ISR_ ## X, CHANGE); \
  } while(0);

#define X ISR
SWITCHES
#undef X

void setupSwitches() {
  #define X(k) ATTACH_ISR(k)
  SWITCHES
  #undef X
}
