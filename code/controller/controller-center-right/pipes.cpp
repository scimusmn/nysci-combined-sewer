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


void createPipes(OctoWS2811 &strip, PipeCollections *pipes) {
  Pipe *vpipe;

  TPIPE(00, showers);
  pipe00->setActivationLevel(1);
  TPIPE(10, toilets);
  pipe10->setActivationLevel(2);
  PIPE(20);
  pipe20->attachInput(pipe00);
  pipe20->attachInput(pipe10);

  TPIPE(30, washers);
  pipe30->setActivationLevel(3);
  TPIPE(40, dishwashers);
  pipe40->setActivationLevel(1);
  PIPE(50);
  pipe50->attachInput(pipe30);
  pipe50->attachInput(pipe40);
  PIPE(60);
  pipe60->attachInput(pipe20);
  pipe60->attachInput(pipe50);
  PIPE(61);
  pipe61->attachInput(pipe60);

  TPIPE(70, showers);
  pipe70->setActivationLevel(2);
  TPIPE(80, toilets);
  pipe80->setActivationLevel(1);
  PIPE(90);
  pipe90->attachInput(pipe61);
  pipe90->attachInput(pipe70);
  pipe90->attachInput(pipe80);

  TPIPE(100, washers);
  pipe100->setActivationLevel(2);
  TPIPE(110, dishwashers);
  pipe110->setActivationLevel(2);
  PIPE(120);
  pipe120->attachInput(pipe100);
  pipe120->attachInput(pipe110);
  TPIPE(130, rains);
  PIPE(140);
  pipe140->attachInput(pipe90);
  pipe140->attachInput(pipe120);
  pipe140->setOverflowSpeed(0);
  PIPE(141);
  pipe141->attachInput(pipe140);

  PIPE(150);
  PIPE(160);
  PIPE(170);
  pipe150->setAsOutput();
  vpipe = new VirtualPipe(strip, 16);
  pipe150->attachInput(pipe130);
  pipe150->attachInput(vpipe);
  vpipe->attachInput(pipe160);
  vpipe = new VirtualPipe(strip, 12);
  pipe160->attachInput(vpipe);
  vpipe->attachInput(pipe141);
  vpipe->attachInput(pipe170);

  PIPE(180);
  PIPE(190);
  PIPE(200);
  pipe180->setAsOutput();
  pipe180->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  pipe180->attachInput(pipe130);
  pipe180->attachInput(pipe190);
  pipe190->attachInput(pipe141);
  pipe190->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  pipe190->attachInput(pipe200);
  pipe200->attachCanInput(4, 270);
  pipe200->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
}
