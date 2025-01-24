#include "flow.h"


#define SEGMENT0 149, 89
#define SEGMENT1 89, 69
#define SEGMENT2 0, 69
#define SEGMENT3 546, 485
#define SEGMENT4 485, 475
#define SEGMENT5 294, 360
#define SEGMENT6 290, 294
#define SEGMENT7 475, 418
#define SEGMENT8 360, 418


PipeSource * pushPipe(Pipe *pipe, PipeSource *list) {
  PipeSource *node = new PipeSource;
  node->pipe = pipe;
  node->next = list;
  return node;
}


void createPipes(OctoWS2811 &strip, PipeCollections *pipes) {
  Pipe *vpipe;
  PIPE(0);
  PIPE(1);
  pipe1->attachInput(pipe0);

  PIPE(2);
  vpipe = new VirtualPipe(strip, 14);
  vpipe->attachCanInput(3, 150);
  pipe2->attachInput(vpipe);

  PIPE(3);
  PIPE(4);
  pipe4->attachInput(pipe3);

  PIPE(5);
  PIPE(6);
  pipe6->attachCanInput(3, 180);
  pipe5->attachInput(pipe6);

  PIPE(7);
  PIPE(8);
  pipe7->attachInput(pipe4);
  pipe8->attachInput(pipe4);
  pipe7->attachInput(pipe5);
  pipe8->attachInput(pipe5);
}
