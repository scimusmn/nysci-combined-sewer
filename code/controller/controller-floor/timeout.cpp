#include <arduino.h>
#include "timeout.h"


bool Timeout::set(unsigned long ms, timefunc_t *f, void* data) {
  if (this->f == nullptr) {
    time = millis() + ms;
    this->f = f;
    this->data = data;
    Serial.println("set!!");
    return true;
  } else {
    Serial.println("not set!! >:c");
    return false;
  }
}


void Timeout::update() {
  if (f != nullptr) {
    if (millis() > time) {
      timefunc_t *ff = f;
      f = nullptr;
      ff(data);
      data = nullptr;
    }
  }
}



bool Timeouts::setTimeout(unsigned long ms, timefunc_t *f, void *data) {
  for (int i=0; i<NUM_TIMEOUTS; i++) {
    if (timeouts[i].set(ms, f, data)) {
      return true;
    }
  }
  return false;
}


void Timeouts::update() {
  for (int i=0; i<NUM_TIMEOUTS; i++) {
    timeouts[i].update();
  }
}
