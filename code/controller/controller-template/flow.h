#pragma once
#include <OctoWS2811.h>
#include "messages.h"

#define N_FLOWS 70
#define N_INPUTS 8
#define N_SEGMENTS 2

#define SEWER_OVERFLOW_SPEED 2
#define DEFAULT_OVERFLOW_SPEED 0.5
#define DEFAULT_DRAIN_SPEED 1


class Pipe;


// linked list for storing collections of pipes
struct PipeSource {
  Pipe *pipe;
  struct PipeSource *next;
};


// pipe collections created by a mapping `pipes.cpp` file
struct PipeCollections {
  PipeSource *pipes;
  PipeSource *rains;
  PipeSource *toilets;
  PipeSource *sinks;
  PipeSource *washers;
  PipeSource *dishwashers;
  PipeSource *showers;
  PipeSource *constant;
};


// represents a specific "flow" within a pipe
struct PipeFlow {
  unsigned int offset = 0;
  unsigned int length = 0;
  unsigned int count = 0;
  unsigned int gradientOffset = 0;
  bool active = false;
  void advance(unsigned int maxOffset, unsigned int speed);
};


struct InputFlow : public PipeFlow {
  void reset(unsigned int count, unsigned int speed);
  void reset();
  void advance(unsigned int maxOffset, unsigned int speed);
};


// handles the input to a pipe
class PipeInput {
  protected:
  friend class Pipe;
  friend class OverflowPipe;
  Pipe *localSource[N_INPUTS];

  bool useCan = false;
  uint8_t canNode;
  unsigned int canPipe;
  bool canDrained = true;

  unsigned int selfCount = 0;
  unsigned int canCount = 0;
  unsigned int prevCount = 0;
  bool changed = false;
  bool increased = false;

  
  public:
  PipeInput();
  void attachInput(Pipe *pipe, unsigned int pipeId);
  void attachCanInput(uint8_t node, unsigned int pipeId);
  void updateCanInput(uint8_t srcNode, CanPipeOutput output);
  void updateCanOverflow(uint8_t srcNode, CanPipeOverflow o);
  unsigned int countFlows(unsigned int pipeId);
  bool countIncreased();
  bool countChanged();
  void setOverflowing();
  void setDraining();
  bool drained();
  bool isOverflowing();
  void setOverflowSpeed(double speed);
};


// handle the output of a pipe
class PipeOutput {
  public:
  PipeOutput(unsigned int id, size_t length);
  void setCanOutput();
  void updateCanOverflow(Pipe *pipe, CanPipeOverflow o);
  void sendDrained();
  unsigned int count();
  void consumeFlow(PipeFlow &flow);
  void flush();

  protected:
  friend class Pipe;

  unsigned int pipeId;
  size_t length;
  unsigned int m_count = 0;
  unsigned int prevCount = 0;
  bool useCan = false;
};



typedef struct {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
} color_t;


// handles pipe drawing
struct PipeRenderer {
  public:
  void configure(OctoWS2811 &strip, size_t start, size_t end);
  void drawFlow(PipeFlow &flow);
  void drawOverflow(double overflowLevel);
  void clear();
  bool active = false;

  protected:
  OctoWS2811 *strip;
  size_t start, end;

  size_t stripIndex(unsigned int i);
  size_t length();
  void drawPixel(int index, color_t color);
};




class Pipe {
  public:
  Pipe(int pipeId, OctoWS2811 &strip, size_t start, size_t end);

  // attach a pipe as input to this pipe
  void attachInput(Pipe *pipe);
  void attachCanInput(uint8_t node, unsigned int pipeId);
  void addSegment(OctoWS2811 &strip, size_t start, size_t end);
  // set whether the pipe should output CAN PipeOutput messages
  void setAsOutput(bool out=true);
  void setActivationLevel(unsigned int level);
  // begin a new flow originating at the start of this pipe
  void startFlow(unsigned int count=1, unsigned int level=0);
  // finish the current flow
  void endFlow();
  // be alerted to new CAN PIPE_OUTPUT messages, only listening to the ones set by attachCanInput
  void updateCanInput(uint8_t node, CanPipeOutput output);
  void updateCanOverflow(uint8_t srcNode, CanPipeOverflow overflow);
  // getters for pipe output
  unsigned int getOutputCount();

  void setOverflowSpeed(double speed);
  void setDrainSpeed(double speed);
  // getter for overflowing
  bool isOverflowing();
  // setter for overflowing
  void setOverflowing();
  // setter for draining
  void setDraining();
  // check if pipe's overflow is empty
  bool drained();


  void update();
  virtual void render();

  protected:
  size_t length;
  unsigned int pipeId;
  PipeInput input;
  PipeOutput output;
  PipeRenderer renderers[N_SEGMENTS];
  PipeFlow movingFlows[N_FLOWS];
  InputFlow inputFlow;
  unsigned int speed = 1;
  bool overflowing = false;
  bool draining = false;
  double overflowLevel = 0;
  double overflowSpeed = DEFAULT_OVERFLOW_SPEED;
  double drainSpeed = DEFAULT_DRAIN_SPEED;
  double maxOverflowLevel = -1;

  unsigned int activationLevel = 1;

  void convertInputToMovingFlow();
  void insertFlow(PipeFlow *f);
  virtual void updateInput();
  virtual void updateOverflow();
};


class VirtualPipe : public Pipe {
  public:
  VirtualPipe(OctoWS2811 &strip, unsigned int length);
  void render();
  void setId(unsigned int pipeId);
};


class OverflowPipe : public Pipe {
  public:
  OverflowPipe(int pipeId, OctoWS2811 &strip, size_t start, size_t end);
  private:
  void updateInput();
  void updateOverflow();
};


void createPipes(OctoWS2811 &strip, PipeCollections *pipes);


#define STARTEND_(number) SEGMENT ## number
#define STARTEND(number) STARTEND_(number)
#define PIPE(number) \
  Pipe * pipe ## number = new Pipe(number, strip, STARTEND(number)); \
  pipes->pipes = pushPipe(pipe ## number, pipes->pipes);
#define TPIPE(number, type) \
  PIPE(number) \
  pipes->type = pushPipe(pipe ## number, pipes->type);


extern Pipe *mainDrain, *overflowDrain, *flowSensor;
