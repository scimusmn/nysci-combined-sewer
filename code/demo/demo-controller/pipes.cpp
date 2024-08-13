#include "flow.h"


// core 0
#define TOILET0_START  0
#define TOILET0_END    5

#define CORE00_START  5
#define CORE00_END    19

#define SINK0_START  19
#define SINK0_END    34

#define CORE01_START  34
#define CORE01_END    48


// core 1
#define WASHER1_START 48
#define WASHER1_END   60


// core 2
#define SHOWER2_START  60
#define SHOWER2_END    70

#define CORE20_START  70
#define CORE20_END    75 

#define TOILET2_START 86
#define TOILET2_END   95

#define CORE21_START  95
#define CORE21_END    100


// core 3
#define SHOWER3_START  111
#define SHOWER3_END    116

#define CORE30_START  75
#define CORE30_END    86 

#define CORE31_START  100
#define CORE31_END    111

#define CORE32_START  116
#define CORE32_END    128




PipeSource * pushPipe(Pipe *pipe, PipeSource *list) {
  PipeSource *node = new PipeSource;
  node->pipe = pipe;
  node->next = list;
  return node;
}


void createPipes(PipeSource **lists, Adafruit_NeoPixel &strip) {
  PipeSource *toilets = nullptr;
  PipeSource *sinks = nullptr;
  PipeSource *washers = nullptr;
  PipeSource *showers = nullptr;
  PipeSource *pipes = nullptr;

  #define PIPE(name) \
    Pipe * name = new Pipe(strip, name ## _START, name ## _END); \
    pipes = pushPipe(name, pipes);
  #define TPIPE(name, type) \
    PIPE(name) \
    type = pushPipe(name, type); \

  TPIPE(TOILET0, toilets);
  TPIPE(SINK0, sinks);
  PIPE(CORE00);
  CORE00->attachInput(TOILET0);
  CORE00->attachInput(SINK0);
  PIPE(CORE01);
  CORE01->attachInput(TOILET0);
  CORE01->attachInput(SINK0);

  TPIPE(WASHER1, washers);

  TPIPE(SHOWER2, showers);
  TPIPE(TOILET2, toilets);
  PIPE(CORE20);
  CORE20->attachInput(SHOWER2);
  CORE20->attachInput(TOILET2);
  PIPE(CORE21);
  CORE21->attachInput(SHOWER2);
  CORE21->attachInput(TOILET2);

  TPIPE(SHOWER3, showers);
  PIPE(CORE30);
  CORE30->attachInput(CORE20);
  CORE30->attachInput(SHOWER3);
  PIPE(CORE31);
  CORE31->attachInput(CORE20);
  CORE31->attachInput(SHOWER3);
  PIPE(CORE32);
  CORE32->attachInput(CORE20);
  CORE32->attachInput(SHOWER3);


  lists[0] = pipes;
  lists[1] = toilets;
  lists[2] = washers;
  lists[3] = sinks;
  lists[4] = showers;
}
