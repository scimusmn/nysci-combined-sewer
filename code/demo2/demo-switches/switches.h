#pragma once

#define RAIN_PIN 16
#define TOILET_PIN 17
#define WASHER_PIN 18
#define DISHWASHER_PIN 14
#define SHOWER_PIN 15


#define SWITCHES \
  X(RAIN_PIN) \
  X(TOILET_PIN) \
  X(WASHER_PIN) \
  X(DISHWASHER_PIN) \
  X(SHOWER_PIN) \

typedef struct {
  bool state;
  unsigned long debounce;
  unsigned int level;
  bool update;
  unsigned long endTime;
} SwitchState;


#define SWITCH(k) SWITCH_(k)
#define SWITCH_(k) switch ## k


#define X(k) extern volatile SwitchState SWITCH(k);
SWITCHES
#undef X



void setupSwitches();
