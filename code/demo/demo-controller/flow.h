#pragma once
#include <OctoWS2811.h>


typedef enum {
  NO_FLOW = 0,
  RAIN = 0x1,
  TOILET = 0x2,
  WASHER = 0x4,
  DISHWASHER = 0x8,
  SHOWER = 0x10,
} FlowType;


class Pipe;


struct PipeSource {
  Pipe *pipe;
  struct PipeSource *next;
};


struct PipeFlow {
  unsigned int type;
  unsigned int offset;
  unsigned int length;
  struct PipeFlow *next;
};


class Pipe {
  public:
  Pipe(OctoWS2811 &strip, size_t start, size_t end);

  // attach a pipe as input to this pipe
  void attachInput(Pipe *pipe);

  // begin a new flow originating at the start of this pipe
  void startFlow(FlowType type, unsigned int rate);
  // finish the current flow
  void endFlow();

  // getters for pipe output
  unsigned int getOutputRate();
  unsigned int getOutputType();

  void update();
  void render();

  protected:
  OctoWS2811 &strip;
  size_t start;
  size_t end;

  struct PipeSource *sources = nullptr;
  struct PipeFlow *flows = nullptr;
  unsigned int speed = 1;
  unsigned int outputRate = 0;
  unsigned int outputType = FlowType::NO_FLOW;

  struct PipeFlow *inputFlow = nullptr;

  FlowType selfType = FlowType::NO_FLOW;
  unsigned int selfRate = 0;

  void processFlow(PipeFlow *flow);
  void updateInput();
};


void createPipes(
  OctoWS2811 &strip,
  PipeSource **pipes,
  PipeSource **toilets,
  PipeSource **sinks,
  PipeSource **washers,
  PipeSource **showers
);
