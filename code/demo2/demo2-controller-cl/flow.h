#pragma once
#include <OctoWS2811.h>

#define N_FLOWS 8
#define N_INPUTS 8


class Pipe;


struct PipeSource {
  Pipe *pipe;
  struct PipeSource *next;
};


struct PipeFlow {
  unsigned int offset;
  unsigned int length;
  unsigned int count;
  bool gradient;
  bool active = true;
  Pipe *src;
  struct PipeFlow *next;
};


class Pipe {
  public:
  Pipe(int pipeId, OctoWS2811 &strip, size_t start, size_t end);

  // attach a pipe as input to this pipe
  void attachInput(Pipe *pipe);

  // begin a new flow originating at the start of this pipe
  void startFlow();
  // finish the current flow
  void endFlow();

  // getters for pipe output
  unsigned int outputCount();

  void update();
  void render();

  protected:
  int pipeId;
  OctoWS2811 &strip;
  size_t start;
  size_t end;

  Pipe *sources[N_INPUTS];
  PipeFlow movingFlows[N_FLOWS];
  PipeFlow inputFlow;

  // struct PipeSource *sources = nullptr;
  // struct PipeFlow *flows = nullptr;
  unsigned int speed = 1;
  void convertInputToMovingFlow();
  void insertFlow(PipeFlow f);

  bool isFlowing = false;
  unsigned int outputFlowing = 0;
  unsigned int flowCount = 0;
  unsigned int selfLength = 0;

  void processFlow(PipeFlow &flow);
  void drawFlow(PipeFlow &flow);
  void updateInput();
  unsigned int stripIndex(unsigned int i);
  unsigned int length();
};


void createPipes(
  OctoWS2811 &strip,
  PipeSource **pipes,
  PipeSource **rains,
  PipeSource **toilets,
  PipeSource **sinks,
  PipeSource **washers,
  PipeSource **showers,
  PipeSource **constant
);


#define STARTEND_(number) SEGMENT ## number
#define STARTEND(number) STARTEND_(number)
#define PIPE(number) \
  Pipe * pipe ## number = new Pipe(number, strip, STARTEND(number)); \
  *pipes = pushPipe(pipe ## number, *pipes);
#define TPIPE(number, type) \
  PIPE(number) \
  *type = pushPipe(pipe ## number, *type); \

