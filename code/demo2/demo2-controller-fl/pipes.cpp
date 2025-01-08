#include "flow.h"

#define SEGMENT1 938, 977
#define SEGMENT2 928, 938
#define SEGMENT3 977, 1023
#define SEGMENT4 840, 927
#define SEGMENT5 1023, 1098
#define SEGMENT6 655, 686
#define SEGMENT7 609, 654
#define SEGMENT8 560, 569
#define SEGMENT9 569, 609
#define SEGMENT10 686, 731
#define SEGMENT11 290, 331
#define SEGMENT12 280, 290
#define SEGMENT13 331, 375
#define SEGMENT14 731, 789
#define SEGMENT15 789, 831
#define SEGMENT16 391, 431
#define SEGMENT17 383, 391
#define SEGMENT18 431, 457
#define SEGMENT19 376, 383
#define SEGMENT20 457, 477
#define SEGMENT21 0, 60
#define SEGMENT22 477, 519
#define SEGMENT23 109, 125
#define SEGMENT24 60, 68
#define SEGMENT25 68, 109
#define SEGMENT26 519, 529
#define SEGMENT27 125, 183


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
  
  TPIPE(1, toilets);
  TPIPE(2, showers);
  PIPE(3);
  pipe3->attachInput(pipe1);
  pipe3->attachInput(pipe2);
  TPIPE(4, dishwashers);
  PIPE(5);
  pipe5->attachInput(pipe3);
  pipe5->attachInput(pipe5);

  TPIPE(6, dishwashers);
  TPIPE(8, showers);
  TPIPE(9, toilets);
  PIPE(7);
  pipe7->attachInput(pipe8);
  pipe7->attachInput(pipe9);
  PIPE(10);
  pipe10->attachInput(pipe5);
  pipe10->attachInput(pipe6);
  pipe10->attachInput(pipe7);

  TPIPE(11, toilets);
  TPIPE(12, showers);
  PIPE(13);
  pipe13->attachInput(pipe11);
  pipe13->attachInput(pipe12);
  TPIPE(14, dishwashers);
  PIPE(15);
  pipe15->attachInput(pipe10);
  pipe15->attachInput(pipe13);
  pipe15->attachInput(pipe14);

  TPIPE(16, toilets);
  TPIPE(17, showers);
  PIPE(18);
  pipe18->attachInput(pipe16);
  pipe18->attachInput(pipe17);
  TPIPE(19, washers);
  PIPE(20);
  pipe20->attachInput(pipe18);
  pipe20->attachInput(pipe19);
  TPIPE(21, dishwashers);
  PIPE(22);
  pipe22->attachInput(pipe15);
  pipe22->attachInput(pipe20);
  pipe22->attachInput(pipe21);

  TPIPE(24, toilets);
  TPIPE(25, toilets);
  PIPE(23);
  pipe23->attachInput(pipe24);
  pipe23->attachInput(pipe25);
  PIPE(26);
  pipe26->attachInput(pipe22);
  pipe26->attachInput(pipe23);
  PIPE(27);
  pipe27->attachInput(pipe26);
}
