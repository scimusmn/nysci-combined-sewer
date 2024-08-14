#pragma once

#define TOILET_PIN 1
#define DISHWASHER_PIN 2
#define SHOWER_PIN 3
#define WASHER_PIN 0

#define SWITCHES \
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
