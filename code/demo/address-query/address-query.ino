#include <OctoWS2811.h>
#include "SerialController.hpp"

SerialController serialController;


// the leds
#define STRIP_LEN 420
DMAMEM int displayMemory[STRIP_LEN*6];
byte pinList[] = { 2, 3, 4, 5, 6, 7, 8, 9 };
OctoWS2811 strip(STRIP_LEN, displayMemory, nullptr, WS2811_RGB | WS2811_800kHz, 8, pinList);

struct span_t {
  int start;
  int end;
} span;


void onParse(char *msg, char *value) {
  int v = atoi(value);
  if (strcmp(msg, "setStart") == 0) {
    span.start = v;
  }
  else if (strcmp(msg, "setEnd") == 0) {
    span.end = v;
  }
  else if (strcmp(msg, "bumpStart") == 0) {
    span.start += v;
  }
  else if (strcmp(msg, "bumpEnd") == 0) {
    span.end += v;
  }
  else {
    Serial.println("unknown command!");
  }
  Serial.print((int) span.start); Serial.print(" <-> "); Serial.println((int) span.end);
}


void setup() {
  serialController.setup(9600, &onParse);
  Serial.println("begin boot...");
  delay(1000);
  Serial.println("booted!");
  strip.begin();
  memset(displayMemory, 0, sizeof(displayMemory));
  strip.show();
}


void loop() {
  // Serial.println("loop");
  serialController.update();

  for (int i=0; i<strip.numPixels(); i++) {
    if (i >= span.start && i < span.end) {
      strip.setPixel(i, 255, 255, 255);
    } else {
      strip.setPixel(i, 0);
    }
  }

  // display
  strip.show();
}//*/
