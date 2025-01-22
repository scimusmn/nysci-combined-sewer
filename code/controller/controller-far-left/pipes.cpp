#include "flow.h"

#define SEGMENT1 0, 84 // //
#define SEGMENT2 87, 97 // //
#define SEGMENT3 98, 137 // //
#define SEGMENT4 137, 182 // //
#define SEGMENT5 182, 257 // //
#define SEGMENT6 291, 299 // //
#define SEGMENT7 299, 338 // //
#define SEGMENT8 338, 383 // //
#define SEGMENT9 385, 416 // //
#define SEGMENT10 416, 460 // //
#define SEGMENT11 461, 518 // //
#define SEGMENT12 581, 590 // //
#define SEGMENT13 590, 628 // //
#define SEGMENT14 628, 675 // //
#define SEGMENT15 518, 560 // //
#define SEGMENT16 870, 928 // //
#define SEGMENT17 675, 683 // //
#define SEGMENT18 683, 691 // //
#define SEGMENT19 691, 731 // //
#define SEGMENT20 731, 757 // //
#define SEGMENT21 757, 776 // //
#define SEGMENT22 776, 817 // //
#define SEGMENT23 938, 977 // //
#define SEGMENT24 930, 938 // //
#define SEGMENT25 977, 995 // //
#define SEGMENT26 995, 1053 // //
#define SEGMENT27 817, 826 //  //
#define SEGMENT28 1160, 1241 // //
#define SEGMENT29 1241, 1279 // //
#define SEGMENT30 1391, 1420 // //
#define SEGMENT31 1335, 1391 // //
#define SEGMENT32 1335, 1279 // //
#define SEGMENT33 1450, 1532 // //
#define SEGMENT34 1532, 1581 //
#define SEGMENT35 1581, 1624 //



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
  TPIPE(3, toilets); //
  TPIPE(2, showers);
  PIPE(4);
  pipe4->attachInput(pipe3);
  pipe4->attachInput(pipe2);
  TPIPE(1, dishwashers);
  PIPE(5);
  pipe5->attachInput(pipe4);
  pipe5->attachInput(pipe1);

  TPIPE(9, dishwashers); //
  TPIPE(6, showers);
  TPIPE(7, toilets);
  PIPE(8);
  pipe8->attachInput(pipe6);
  pipe8->attachInput(pipe7);
  PIPE(10);
  pipe10->attachInput(pipe9);
  pipe10->attachInput(pipe5);
  pipe10->attachInput(pipe8);

  TPIPE(13, toilets); //
  TPIPE(12, showers);
  PIPE(14);
  pipe14->attachInput(pipe13);
  pipe14->attachInput(pipe12);
  TPIPE(11, dishwashers);
  PIPE(15);
  pipe15->attachInput(pipe10);
  pipe15->attachInput(pipe11);
  pipe15->attachInput(pipe14);

  TPIPE(19, toilets); //
  TPIPE(18, showers);
  PIPE(20);
  pipe20->attachInput(pipe19);
  pipe20->attachInput(pipe18);
  TPIPE(17, washers);
  PIPE(21);
  pipe21->attachInput(pipe17);
  pipe21->attachInput(pipe20);
  TPIPE(16, dishwashers);
  PIPE(22);
  pipe22->attachInput(pipe15);
  pipe22->attachInput(pipe16);
  pipe22->attachInput(pipe21);

  TPIPE(23, toilets);
  TPIPE(24, toilets);
  PIPE(25);
  pipe25->attachInput(pipe23);
  pipe25->attachInput(pipe24);
  PIPE(27);
  pipe27->attachInput(pipe22);
  pipe27->attachInput(pipe25);
  PIPE(26);
  pipe26->attachInput(pipe27);

  TPIPE(28, constant);
  PIPE(29);
  pipe29->attachInput(pipe28);
  PIPE(30);
  pipe30->attachInput(pipe29);

  TPIPE(31, rains);
  TPIPE(32,rains);
  TPIPE(33,constant);
  PIPE(34);
  pipe34->attachInput(pipe33);
  pipe34->attachInput(pipe26);
  PIPE(35);
  pipe35->attachInput(pipe31);
  pipe35->attachInput(pipe34);
}
