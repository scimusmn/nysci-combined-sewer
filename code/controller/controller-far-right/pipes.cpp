#include "flow.h"

#define SEGMENT0 118, 149 
#define SEGMENT1 110, 118 
#define SEGMENT2 0, 8 
#define SEGMENT3 8, 53 
#define SEGMENT4 86, 110 
#define SEGMENT5 53, 86 
#define SEGMENT6 151, 272 
#define SEGMENT7 408, 439 
#define SEGMENT8 290, 298 
#define SEGMENT9 298, 306 
#define SEGMENT10 306, 352 
#define SEGMENT11 386, 404 
#define SEGMENT12 352, 386 
#define SEGMENT13 439, 514 
#define SEGMENT14 580, 588 
#define SEGMENT15 588, 596 
#define SEGMENT16 596, 642 
#define SEGMENT17 676, 694 
#define SEGMENT18 643, 676 
#define SEGMENT19 514, 554 
#define SEGMENT20 697, 728 
#define SEGMENT21 1109, 1076 
#define SEGMENT22 1046, 1076 
#define SEGMENT23 869, 924
#define SEGMENT24 955, 924 
#define SEGMENT25 955, 985 
#define SEGMENT26 985, 1046 
#define SEGMENT27 1160, 1215 
#define SEGMENT28 1215,1254 
#define SEGMENT29 1254,1299 

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
  PipeSource **showers,
  PipeSource **constant
) {
  #define STARTEND_(number) SEGMENT ## number
  #define STARTEND(number) STARTEND_(number)
  #define PIPE(number) \
    Pipe * pipe ## number = new Pipe(number, strip, STARTEND(number)); \
    *pipes = pushPipe(pipe ## number, *pipes);
  #define TPIPE(number, type) \
    PIPE(number) \
    *type = pushPipe(pipe ## number, *type); \

  TPIPE(0, washers); //
  TPIPE(1, dishwashers); //
  TPIPE(2, showers); //
  TPIPE(3, toilets); //
  PIPE(5); 
  pipe5->attachInput(pipe3);
  pipe5->attachInput(pipe2);
  PIPE(4);
  pipe4->attachInput(pipe5);
  pipe4->attachInput(pipe1);
  PIPE(6);
  pipe6->attachInput(pipe0);
  pipe6->attachInput(pipe4);

  TPIPE(7, washers); //
  TPIPE(8, dishwashers); //
  TPIPE(9, showers); //
  TPIPE(10, toilets); //
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

  TPIPE(26, rains);
  //TPIPE(25, rains);
  //PIPE(24);
  //pipe24->attachInput(pipe25);
  //PIPE(27);
  PIPE(28);
  //pipe28->attachInput(pipe27);
  pipe28->attachInput(pipe26);
  PIPE(29);
  pipe29->attachInput(pipe28);
  pipe29->attachInput(pipe20);

  TPIPE(23, constant);
  PIPE(22);
  pipe22->attachInput(pipe23);
  PIPE(21);
  pipe21->attachInput(pipe22);


}
