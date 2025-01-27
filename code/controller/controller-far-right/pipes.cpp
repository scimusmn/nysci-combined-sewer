#include "flow.h"

#define SEGMENT0 118, 149 //
#define SEGMENT10 110, 118 //
#define SEGMENT20 0, 8 //
#define SEGMENT30 8, 53 //
#define SEGMENT40 86, 110 //
#define SEGMENT50 53, 86 //
#define SEGMENT60 151, 272 //

#define SEGMENT70 408, 439 //
#define SEGMENT80 290, 298 //
#define SEGMENT90 298, 306 // 
#define SEGMENT100 306, 352 //
#define SEGMENT110 386, 404 //
#define SEGMENT120 352, 386 //
#define SEGMENT130 439, 514 //

#define SEGMENT140 580, 588 //
#define SEGMENT150 588, 596 //
#define SEGMENT160 596, 642 //
#define SEGMENT170 676, 694 //
#define SEGMENT180 643, 676 //
#define SEGMENT190 514, 554 //
#define SEGMENT191 697, 728 //

#define SEGMENT200 1109, 1076 //
#define SEGMENT210 1046, 1076 //
#define SEGMENT220 869, 924 //

#define SEGMENT230 955, 985 //
#define SEGMENT231 955, 924 //
#define SEGMENT240 985, 1046 //

#define SEGMENT250 1160, 1215 //
#define SEGMENT260 1215,1254 //
#define SEGMENT270 1254,1299 //

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
  TPIPE(10, dishwashers); //
  TPIPE(20, showers); //
  TPIPE(30, toilets); //
  PIPE(50); 
  pipe50->attachInput(pipe30);
  pipe50->attachInput(pipe20);
  PIPE(40);
  pipe40->attachInput(pipe50);
  pipe40->attachInput(pipe10);
  PIPE(60);
  pipe60->attachInput(pipe0);
  pipe60->attachInput(pipe40);

  TPIPE(70, washers); //
  TPIPE(80, dishwashers); //
  TPIPE(90, showers); //
  TPIPE(100, toilets); //
  PIPE(120);
  pipe120->attachInput(pipe100);
  pipe120->attachInput(pipe90);
  PIPE(110);
  pipe110->attachInput(pipe120);
  pipe110->attachInput(pipe80);
  PIPE(130);
  pipe130->attachInput(pipe60);
  pipe130->attachInput(pipe110);
  pipe130->attachInput(pipe70);

  TPIPE(140, dishwashers);
  TPIPE(150, dishwashers);
  TPIPE(160, toilets);
  PIPE(180);
  pipe180->attachInput(pipe150);
  pipe180->attachInput(pipe160);
  PIPE(170);
  pipe170->attachInput(pipe180);
  pipe170->attachInput(pipe140);
  PIPE(190);
  pipe190->attachInput(pipe130);
  pipe190->attachInput(pipe170);
  PIPE(191);
  pipe191->attachInput(pipe190);

  TPIPE(240, rains);
  TPIPE(230, rains);
  PIPE(231);
  pipe231->attachInput(pipe20);

  TPIPE(250,constant);
  PIPE(260);
  pipe260->attachInput(pipe250);
  pipe260->attachInput(pipe240);
  PIPE(270);
  pipe270->attachInput(pipe260);
  pipe270->attachInput(pipe191);

  TPIPE(220, constant);
  PIPE(210);
  pipe210->attachInput(pipe220);
  PIPE(200);
  pipe200->attachInput(pipe210);


}
