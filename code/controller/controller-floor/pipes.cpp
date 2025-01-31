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


Pipe* overflowMe;
Pipe* overflowMeToo;


PipeSource * pushPipe(Pipe *pipe, PipeSource *list) {
  PipeSource *node = new PipeSource;
  node->pipe = pipe;
  node->next = list;
  return node;
}


void createPipes(OctoWS2811 &strip, PipeCollections *pipes) {
  Pipe *vpipe;
  PIPE(10);
  pipe10->addSegment(strip, SEGMENT20);
  pipe10->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  //flowSensor = pipe10;
  // PIPE(20);
  // overflowInput = pipe20;
  pipe10->attachCanInput(5,8);
  // pipe20->attachCanInput(5,8);

  PIPE(100);
  pipe100->attachInput(pipe10);
  pipe100->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  PIPE(110);
  pipe110->attachInput(pipe100);
  pipe110->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  pipe110->addSegment(strip, SEGMENT120);
  PIPE(130);
  pipe130->attachInput(pipe110);
  pipe130->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  PIPE(140);
  pipe140->attachInput(pipe130);
  pipe140->addSegment(strip, SEGMENT150);
  pipe140->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  PIPE(160);
  pipe160->attachInput(pipe140);
  pipe160->addSegment(strip, SEGMENT170);
  pipe160->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  PIPE(161);
  pipe161->attachInput(pipe160);
  pipe161->addSegment(strip, SEGMENT171);
  pipe161->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  PIPE(180);
  pipe180->attachInput(pipe161);
  pipe180->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  PIPE(190);
  pipe190->attachInput(pipe180);
  pipe190->addSegment(strip, SEGMENT200);
  pipe190->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  mainDrain = pipe190;


  OverflowPipe *pipe30 = new OverflowPipe(30, strip, SEGMENT30);
  pipes->pipes = pushPipe(pipe30, pipes->pipes);
  // PIPE(30);
  pipe30->attachInput(pipe10);
  pipe30->addSegment(strip, SEGMENT40);
  pipe30->setOverflowSpeed(SEWER_OVERFLOW_SPEED);

  PIPE(50);
  pipe50->attachInput(pipe30);
  pipe50->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  PIPE(51);
  pipe51->attachInput(pipe50);
  pipe51->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  pipe51->addSegment(strip, SEGMENT60);
  PIPE(70);
  pipe70->attachInput(pipe51);
  pipe70->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  PIPE(90);
  pipe90->attachInput(pipe70);
  pipe90->addSegment(strip, SEGMENT80);
  pipe90->setOverflowSpeed(SEWER_OVERFLOW_SPEED);
  overflowDrain = pipe90;
}
