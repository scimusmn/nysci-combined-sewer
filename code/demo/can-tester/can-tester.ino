#include "messages.h"

void processInputLevels(uint8_t src, InputLevels levels) {
  Serial.println("new levels!!!");
}


void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("[cantest] boot!");
  setupCan();
  Serial.println("can configured");
}


void loop() {}
