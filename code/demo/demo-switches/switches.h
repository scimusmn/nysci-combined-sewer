#pragma once

#define RAIN_PIN 0
#define TOILET_PIN 1
#define WASHER_PIN 2
#define DISHWASHER_PIN 3
#define SHOWER_PIN 4


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
