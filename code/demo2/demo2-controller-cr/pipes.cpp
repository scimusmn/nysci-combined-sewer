#include "flow.h"

#define SEGMENT1 0, 59
#define SEGMENT2 126, 158
#define SEGMENT3 158, 206
#define SEGMENT4 67, 106
#define SEGMENT5 59, 67
#define SEGMENT6 106, 126
#define SEGMENT7 206, 240
#define SEGMENT8 280, 351
#define SEGMENT9 351, 410
#define SEGMENT10 477, 508
#define SEGMENT11 508, 560
#define SEGMENT12 418, 458
#define SEGMENT13 410, 418
#define SEGMENT14 458, 477


PipeSource * pushPipe(Pipe *pipe, PipeSource *list) {
  PipeSource *node = new PipeSource;
  node->pipe = pipe;
  node->next = list;
  return node;
}


void createPipes(
  OctoWS2811 &strip,
  PipeSource **pipes,
  PipeSource **rains,
  PipeSource **toilets,
  PipeSource **washers,
  PipeSource **dishwashers,
  PipeSource **showers
) {
  #define STARTEND_(number) SEGMENT ## number
  #define STARTEND(number) STARTEND_(number)
  #define PIPE(number) \
    Pipe * pipe ## number = new Pipe(strip, STARTEND(number)); \
    *pipes = pushPipe(pipe ## number, *pipes);
  #define TPIPE(number, type) \
    PIPE(number) \
    *type = pushPipe(pipe ## number, *type); \

  TPIPE(1, showers);  
  TPIPE(2, toilets);  
  PIPE(3);
  pipe3->attachInput(pipe1);
  pipe3->attachInput(pipe2);

  TPIPE(4, washers);
  TPIPE(5, dishwashers);
  PIPE(6);
  pipe6->attachInput(pipe4);
  pipe6->attachInput(pipe5);
  PIPE(7);
  pipe7->attachInput(pipe6);
  pipe7->attachInput(pipe3);
  PIPE(8);
  pipe8->attachInput(pipe7);

  TPIPE(9, showers);
  TPIPE(10, toilets);
  PIPE(11);
  pipe11->attachInput(pipe8);
  pipe11->attachInput(pipe9);
  pipe11->attachInput(pipe10);

  TPIPE(12, washers);
  TPIPE(13, dishwashers);
}
