#pragma once
#include <OctoWS2811.h>
#include "messages.h"

#define N_FLOWS 16 
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
  unsigned int gradientOffset;
  bool active = true;
  Pipe *src;
  struct PipeFlow *next;
};


class Pipe {
  public:
  Pipe(int pipeId, OctoWS2811 &strip, size_t start, size_t end);

  // attach a pipe as input to this pipe
  void attachInput(Pipe *pipe);
  void attachCanInput(uint8_t node, unsigned int pipeId);

  // set whether the pipe should output CAN PipeOutput messages
  void setAsOutput(bool out=true);

  // begin a new flow originating at the start of this pipe
  void startFlow(unsigned int count=1);
  // finish the current flow
  void endFlow();

  // be alerted to new CAN PIPE_OUTPUT messages, only listening to the ones set by attachCanInput
  void updateCanInput(uint8_t node, PipeOutput output);

  // getters for pipe output
  unsigned int getOutputCount();

  void update();
  virtual void render();

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

  unsigned int selfInputCount = 0;
  unsigned int totalInputCount = 0;
  unsigned int outputCount = 0;

  bool canOutput = false;
  int canInputId = -1;
  int canInputPipe = -1;
  unsigned int canInputFlow = 0;

  void processFlow(PipeFlow &flow);
  void drawFlow(PipeFlow &flow);
  void updateInput();
  unsigned int stripIndex(unsigned int i);
  virtual unsigned int length();
};


class VirtualPipe : public Pipe {
  public:
  VirtualPipe(OctoWS2811 &strip, unsigned int length);
  void render();
  private:
  unsigned int length();
  unsigned int len;
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
  *type = pushPipe(pipe ## number, *type);
#define VPIPE(len) \
  vpipe = new VirtualPipe(strip, len); \
  *pipes = pushPipe(vpipe, *pipes);
