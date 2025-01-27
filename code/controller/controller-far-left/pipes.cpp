#include "flow.h"

#define SEGMENT0 98, 137 // //
#define SEGMENT10 87, 97 // //
#define SEGMENT20 0, 84 // //
#define SEGMENT30 137, 182 // //
#define SEGMENT40 182, 257 // //

#define SEGMENT50 385, 416 // //
#define SEGMENT60 291, 299 // //
#define SEGMENT70 299, 338 // //
#define SEGMENT80 338, 383 // //
#define SEGMENT90 416, 460 // //

#define SEGMENT100 590, 628 // //
#define SEGMENT110 581, 590 // //
#define SEGMENT120 461, 518 // //
#define SEGMENT130 628, 675 // //
#define SEGMENT140 518, 560 // //

#define SEGMENT150 691, 731 // //
#define SEGMENT160 683, 691 // //
#define SEGMENT170 675, 683 // //
#define SEGMENT180 870, 928 // //
#define SEGMENT190 731, 757 // //
#define SEGMENT200 757, 776 // //
#define SEGMENT210 776, 817 // //

#define SEGMENT220 930, 938 // //
#define SEGMENT230 938, 977 // //
#define SEGMENT240 817, 826 //  //
#define SEGMENT241 995, 1053 // //
#define SEGMENT250 977, 995 // //

#define SEGMENT260 1160, 1241 // //
#define SEGMENT270 1241, 1279 // //
#define SEGMENT280 1391, 1420 // //

#define SEGMENT290 1335, 1391 // //
#define SEGMENT300 1335, 1279 // //

#define SEGMENT310 1450, 1532 // //
#define SEGMENT320 1532, 1581 //
#define SEGMENT330 1581, 1624 //



PipeSource * pushPipe(Pipe *pipe, PipeSource *list) {
  PipeSource *node = new PipeSource;
  node->pipe = pipe;
  node->next = list;
  return node;
}


void createPipes(OctoWS2811 &strip, PipeCollections *pipes) {
  TPIPE(0, toilets); //
  TPIPE(10, showers);
  PIPE(30);
  pipe30->attachInput(pipe0);
  pipe30->attachInput(pipe10);
  TPIPE(20, dishwashers);
  PIPE(40);
  pipe40->attachInput(pipe30);
  pipe40->attachInput(pipe20);

  TPIPE(50, dishwashers); //
  TPIPE(60, showers);
  TPIPE(70, toilets);
  PIPE(80);
  pipe80->attachInput(pipe60);
  pipe80->attachInput(pipe70);
  PIPE(90);
  pipe90->attachInput(pipe50);
  pipe90->attachInput(pipe40);
  pipe90->attachInput(pipe80);

  TPIPE(100, toilets); //
  TPIPE(110, showers);
  PIPE(130);
  pipe130->attachInput(pipe100);
  pipe130->attachInput(pipe110);
  TPIPE(120, dishwashers);
  PIPE(140);
  pipe140->attachInput(pipe90);
  pipe140->attachInput(pipe120);
  pipe140->attachInput(pipe130);

  TPIPE(150, toilets); //
  TPIPE(160, showers);
  PIPE(190);
  pipe190->attachInput(pipe150);
  pipe190->attachInput(pipe160);
  TPIPE(170, washers);
  PIPE(200);
  pipe200->attachInput(pipe170);
  pipe200->attachInput(pipe190);
  TPIPE(180, dishwashers);
  PIPE(210);
  pipe210->attachInput(pipe140);
  pipe210->attachInput(pipe180);
  pipe210->attachInput(pipe200);

  TPIPE(230, toilets);
  TPIPE(220, toilets);
  PIPE(250);
  pipe250->attachInput(pipe230);
  pipe250->attachInput(pipe220);
  PIPE(240);
  pipe240->attachInput(pipe210);
  pipe240->attachInput(pipe250);
  PIPE(241);
  pipe241->attachInput(pipe240);

  // TPIPE(260, constant);
  // PIPE(270);
  // pipe270->attachInput(pipe260);
  // PIPE(280);
  // pipe280->attachInput(pipe270);

  TPIPE(290, rains);
  TPIPE(300,rains);
  TPIPE(310,constant);
  PIPE(320);
  pipe320->attachInput(pipe310);
  pipe320->attachInput(pipe241);
  PIPE(330);
  pipe330->attachInput(pipe290);
  pipe330->attachInput(pipe320);
  pipe330->setAsOutput();
}
