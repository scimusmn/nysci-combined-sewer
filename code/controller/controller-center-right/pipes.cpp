#include "flow.h"


#define SEGMENT00 0, 59
#define SEGMENT10 126, 157
#define SEGMENT20 157, 203
#define SEGMENT30 67, 105
#define SEGMENT40 59, 76
#define SEGMENT50 105, 126
#define SEGMENT60 203, 240
#define SEGMENT61 290, 361
#define SEGMENT70 361, 420
#define SEGMENT80 487, 518
#define SEGMENT90 518, 567
#define SEGMENT100 428, 467
#define SEGMENT110 420, 428
#define SEGMENT120 467, 487
#define SEGMENT130 926, 981
#define SEGMENT140 567, 579
#define SEGMENT141 580, 637
#define SEGMENT150 981, 992
#define SEGMENT160 898, 926
#define SEGMENT170 870, 898
#define SEGMENT180 1060, 1087
#define SEGMENT190 1020, 1060
#define SEGMENT200 993, 1020



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
  PipeSource **constants
) {
  TPIPE(00, showers);
  TPIPE(10, toilets);
  PIPE(20);
  pipe20->attachInput(pipe00);
  pipe20->attachInput(pipe10);

  TPIPE(30, washers);
  TPIPE(40, dishwashers);
  PIPE(50);
  pipe50->attachInput(pipe30);
  pipe50->attachInput(pipe40);
  PIPE(60);
  pipe60->attachInput(pipe20);
  pipe60->attachInput(pipe50);
  PIPE(61);
  pipe61->attachInput(pipe60);

  TPIPE(70, showers);
  TPIPE(80, toilets);
  PIPE(90);
  pipe90->attachInput(pipe61);
  pipe90->attachInput(pipe70);
  pipe90->attachInput(pipe80);

  TPIPE(100, washers);
  TPIPE(110, dishwashers);
  PIPE(120);
  pipe120->attachInput(pipe100);
  pipe120->attachInput(pipe110);
  TPIPE(130, rains);
  PIPE(140);
  pipe140->attachInput(pipe90);
  pipe140->attachInput(pipe120);
  PIPE(141);
  pipe141->attachInput(pipe140);

  TPIPE(150, constants);
  TPIPE(160, constants);
  TPIPE(170, constants);

  PIPE(180);
  PIPE(190);
  PIPE(200);
  pipe180->attachInput(pipe130);
  pipe180->attachInput(pipe190);
  pipe190->attachInput(pipe141);
  pipe190->attachInput(pipe200);
}
