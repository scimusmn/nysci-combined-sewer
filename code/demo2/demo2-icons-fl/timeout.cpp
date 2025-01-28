#include "timeout.h"


bool Timeout::set(unsigned long ms, timefunc_t f, void* data) {
  if (f == nullptr) {
    time = millis() + ms;
    this->f = f;
    this->data = data;
    return true;
  } else {
    return false;
  }
}


void Timeout::update() {
  if (f != nullptr) {
    if (millis() > time) {
      f(data);
      f = nullptr;
      data = nullptr;
    }
  }
}



bool Timeouts::setTimeout(unsigned long ms, timefunc_t f, void *data) {
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
