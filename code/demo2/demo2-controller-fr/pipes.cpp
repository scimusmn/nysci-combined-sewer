#include "flow.h"

#define SEGMENT0 118, 151
#define SEGMENT1 110, 118
#define SEGMENT2 0, 8
#define SEGMENT3 8, 53
#define SEGMENT4 86, 110
#define SEGMENT5 53, 86
#define SEGMENT6 151, 271
#define SEGMENT7 398, 431
#define SEGMENT8 280, 288
#define SEGMENT9 288, 296
#define SEGMENT10 296, 340
#define SEGMENT11 375, 398
#define SEGMENT12 340, 375
#define SEGMENT13 431, 502
#define SEGMENT14 560, 568
#define SEGMENT15 568, 576
#define SEGMENT16 576, 621
#define SEGMENT17 654, 677
#define SEGMENT18 621, 654
#define SEGMENT19 502, 544
#define SEGMENT20 677, 708


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

  TPIPE(0, washers);
  TPIPE(1, dishwashers);
  TPIPE(2, showers);
  TPIPE(3, toilets);
  PIPE(5);
  pipe5->attachInput(pipe3);
  pipe5->attachInput(pipe2);
  PIPE(4);
  pipe4->attachInput(pipe5);
  pipe4->attachInput(pipe1);
  PIPE(6);
  pipe6->attachInput(pipe0);
  pipe6->attachInput(pipe4);

  TPIPE(7, washers);
  TPIPE(8, dishwashers);
  TPIPE(9, showers);
  TPIPE(10, toilets);
  PIPE(12);
  pipe12->attachInput(pipe10);
  pipe12->attachInput(pipe9);
  PIPE(11);
  pipe11->attachInput(pipe12);
  pipe11->attachInput(pipe8);
  PIPE(13);
  pipe13->attachInput(pipe6);
  pipe13->attachInput(pipe11);
  pipe13->attachInput(pipe7);

  TPIPE(14, dishwashers);
  TPIPE(15, dishwashers);
  TPIPE(16, toilets);
  PIPE(18);
  pipe18->attachInput(pipe15);
  pipe18->attachInput(pipe16);
  PIPE(17);
  pipe17->attachInput(pipe18);
  pipe17->attachInput(pipe14);
  PIPE(19);
  pipe19->attachInput(pipe13);
  pipe19->attachInput(pipe17);
  PIPE(20);
  pipe20->attachInput(pipe19);
}
