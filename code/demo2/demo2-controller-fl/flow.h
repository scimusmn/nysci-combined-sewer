#pragma once
#include <OctoWS2811.h>


class Pipe;


struct PipeSource {
  Pipe *pipe;
  struct PipeSource *next;
};


struct PipeFlow {
  unsigned int offset;
  unsigned int length;
  Pipe *src;
  struct PipeFlow *next;
};


class Pipe {
  public:
  Pipe(OctoWS2811 &strip, size_t start, size_t end);

  // attach a pipe as input to this pipe
  void attachInput(Pipe *pipe);

  // begin a new flow originating at the start of this pipe
  void startFlow();
  // finish the current flow
  void endFlow();

  // getters for pipe output
  unsigned int outputIsFlowing();

  void update();
  void render();

  protected:
  OctoWS2811 &strip;
  size_t start;
  size_t end;

  struct PipeSource *sources = nullptr;
  struct PipeFlow *flows = nullptr;
  unsigned int speed = 1;
  PipeFlow *outputFlow;

  struct PipeFlow *inputFlow = nullptr;
  void removeInputFlow();

  bool isFlowing = false;
  bool outputFlowing = false;
  unsigned int flowCount = 0;
  unsigned int selfLength = 0;

  void processFlow(PipeFlow *flow);
  void updateInput();
};


void createPipes(
  OctoWS2811 &strip,
  PipeSource **pipes,
  PipeSource **rains,
  PipeSource **toilets,
  PipeSource **sinks,
  PipeSource **washers,
  PipeSource **showers
);
