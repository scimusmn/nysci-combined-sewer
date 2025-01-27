#include "flow.h"


#define SEGMENT10 0,39
#define SEGMENT20 290, 330

#define SEGMENT30 39, 50
#define SEGMENT40 433, 423

#define SEGMENT50 50, 213
#define SEGMENT51 580, 589
#define SEGMENT60 870, 879

#define SEGMENT70 589, 651

#define SEGMENT80 651, 765
#define SEGMENT90 879, 993

#define SEGMENT100 330, 367

#define SEGMENT110 367, 412
#define SEGMENT120 433, 480

#define SEGMENT130 480, 540

#define SEGMENT140 412, 423
#define SEGMENT150 540, 550

#define SEGMENT160 1160, 1382
#define SEGMENT161 1740, 1750
#define SEGMENT170 1450, 1673
#define SEGMENT171 2030, 2040

#define SEGMENT180 1750, 1810

#define SEGMENT190 1810, 1842
#define SEGMENT200 2040, 2071


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
  PipeSource **constant,
  PipeSource **overflow
) {
  #define STARTEND_(number) SEGMENT ## number
  #define STARTEND(number) STARTEND_(number)
  #define PIPE(number) \
    Pipe * pipe ## number = new Pipe(number, strip, STARTEND(number)); \
    *pipes = pushPipe(pipe ## number, *pipes);
  #define TPIPE(number, type) \
    PIPE(number) \
    *type = pushPipe(pipe ## number, *type); \
  
  TPIPE(10,constant);
  TPIPE(20,constant);

  PIPE(100);
  pipe100->attachInput(pipe20);
  PIPE(110);
  pipe110->attachInput(pipe100);
  PIPE(120);
  pipe120->attachInput(pipe100);
  PIPE(130);
  pipe130->attachInput(pipe110);
  PIPE(140);
  pipe140->attachInput(pipe130);
  PIPE(150);
  pipe150->attachInput(pipe130);
  PIPE(160);
  pipe160->attachInput(pipe140);
  PIPE(161);
  pipe161->attachInput(pipe160);
  PIPE(170);
  pipe170->attachInput(pipe140);
  PIPE(171);
  pipe171->attachInput(pipe170);
  PIPE(180);
  pipe180->attachInput(pipe161);
  PIPE(190);
  pipe190->attachInput(pipe180);
  PIPE(200);
  pipe200->attachInput(pipe180);



  TPIPE(40,overflow);
  TPIPE(30,overflow);

  PIPE(50);
  pipe50->attachInput(pipe30);
  PIPE(51);
  pipe51->attachInput(pipe50);
  PIPE(60);
  pipe60->attachInput(pipe50);
  PIPE(70);
  pipe70->attachInput(pipe51);
  PIPE(80);
  pipe80->attachInput(pipe70);
  PIPE(90);
  pipe90->attachInput(pipe70);
}
