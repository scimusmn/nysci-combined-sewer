#include "flow.h"


// top segment
#define FLOW_TOP_0_START 119
#define FLOW_TOP_0_END   149

#define FLOW_TOP_1_START 0
#define FLOW_TOP_1_END   10

#define FLOW_TOP_2_START 10
#define FLOW_TOP_2_END   20

#define FLOW_TOP_3_START 20
#define FLOW_TOP_3_END   71

#define GUTTER_TOP_1_START 71
#define GUTTER_TOP_1_END   104

#define GUTTER_TOP_0_START 104
#define GUTTER_TOP_0_END   118

#define COL_TOP_START 149
#define COL_TOP_END   185

// upper middle segment
#define COL_UM_0_START 452
#define COL_UM_0_END   544

#define FLOW_UM_0_START 419
#define FLOW_UM_0_END   452

#define FLOW_UM_1_START 300
#define FLOW_UM_1_END   310

#define FLOW_UM_2_START 310
#define FLOW_UM_2_END   320

#define FLOW_UM_3_START 320
#define FLOW_UM_3_END   371

#define GUTTER_UM_1_START 371
#define GUTTER_UM_1_END   404

#define GUTTER_UM_0_START 404
#define GUTTER_UM_0_END   419

#define COL_UM_1_START 544
#define COL_UM_1_END   560


// lower middle segment
#define COL_LM_0_START 600
#define COL_LM_0_END   667

#define FLOW_LM_0_START 709
#define FLOW_LM_0_END   720

#define FLOW_LM_1_START 720
#define FLOW_LM_1_END   731

#define FLOW_LM_2_START 731
#define FLOW_LM_2_END   780

#define GUTTER_LM_1_START 780
#define GUTTER_LM_1_END   813

#define GUTTER_LM_0_START 813
#define GUTTER_LM_0_END   831

#define COL_LM_1_START 667
#define COL_LM_1_END   709

#define FLOW_LM_3_START 831
#define FLOW_LM_3_END   863


// bottom segment
#define COL_BTM_0_START 900
#define COL_BTM_0_END   925

#define COL_BTM_1_START 925
#define COL_BTM_1_END   952

#define GUTTER_BTM_7_START 952
#define GUTTER_BTM_7_END   1006

#define GUTTER_BTM_6_START 1006
#define GUTTER_BTM_6_END   1018

#define GUTTER_BTM_5_START 1018
#define GUTTER_BTM_5_END   1050

#define GUTTER_BTM_4_START 1050
#define GUTTER_BTM_4_END   1057

#define GUTTER_BTM_3_START 1057
#define GUTTER_BTM_3_END   1088

#define GUTTER_BTM_2_START 1088
#define GUTTER_BTM_2_END   1140

#define GUTTER_BTM_1_START 1140
#define GUTTER_BTM_1_END   1169

#define GUTTER_BTM_0_START 1169
#define GUTTER_BTM_0_END   1200

/*
#define GUTTER_BTM_1_START 1050
#define GUTTER_BTM_1_END   1088

#define GUTTER_BTM_6_START 1088
#define GUTTER_BTM_6_END   1140

#define GUTTER_BTM_4_START 1140
#define GUTTER_BTM_4_END   1168
*/


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
  #define PIPE(name) \
    Pipe * name = new Pipe(strip, name ## _START, name ## _END); \
    *pipes = pushPipe(name, *pipes);
  #define TPIPE(name, type) \
    PIPE(name) \
    *type = pushPipe(name, *type); \

  // top
  TPIPE(FLOW_TOP_0, toilets);
  TPIPE(FLOW_TOP_1, washers);
  TPIPE(FLOW_TOP_2, dishwashers);
  TPIPE(FLOW_TOP_3, showers);
  PIPE(GUTTER_TOP_1);
  GUTTER_TOP_1->attachInput(FLOW_TOP_2);
  GUTTER_TOP_1->attachInput(FLOW_TOP_3);
  PIPE(GUTTER_TOP_0);
  GUTTER_TOP_0->attachInput(FLOW_TOP_1);
  GUTTER_TOP_0->attachInput(GUTTER_TOP_1);
  PIPE(COL_TOP);
  COL_TOP->attachInput(GUTTER_TOP_0);
  COL_TOP->attachInput(FLOW_TOP_0);

  // upper middle
  PIPE(COL_UM_0);
  COL_UM_0->attachInput(COL_TOP);
  TPIPE(FLOW_UM_0, toilets);
  TPIPE(FLOW_UM_1, washers);
  TPIPE(FLOW_UM_2, dishwashers);
  TPIPE(FLOW_UM_3, showers);
  PIPE(GUTTER_UM_1);
  GUTTER_UM_1->attachInput(FLOW_UM_2);
  GUTTER_UM_1->attachInput(FLOW_UM_3);
  PIPE(GUTTER_UM_0);
  GUTTER_UM_0->attachInput(FLOW_UM_1);
  GUTTER_UM_0->attachInput(GUTTER_UM_1);
  PIPE(COL_UM_1);
  COL_UM_1->attachInput(GUTTER_UM_0);
  COL_UM_1->attachInput(COL_UM_0);
  COL_UM_1->attachInput(FLOW_UM_0);

  // lower middle
  PIPE(COL_LM_0);
  COL_LM_0->attachInput(COL_UM_1);
  TPIPE(FLOW_LM_0, washers);
  TPIPE(FLOW_LM_1, dishwashers);
  TPIPE(FLOW_LM_2, showers);
  PIPE(GUTTER_LM_1);
  GUTTER_LM_1->attachInput(FLOW_LM_1);
  GUTTER_LM_1->attachInput(FLOW_LM_2);
  PIPE(GUTTER_LM_0);
  GUTTER_LM_0->attachInput(FLOW_LM_0);
  GUTTER_LM_0->attachInput(GUTTER_LM_1);
  PIPE(COL_LM_1);
  COL_LM_1->attachInput(COL_LM_0);
  COL_LM_1->attachInput(GUTTER_LM_0);
  TPIPE(FLOW_LM_3, toilets);

  // bottom
  PIPE(COL_BTM_0);
  COL_BTM_0->attachInput(COL_LM_1);
  PIPE(COL_BTM_1);
  COL_BTM_1->attachInput(FLOW_LM_3);

  PIPE(GUTTER_BTM_7);
  PIPE(GUTTER_BTM_6);
  GUTTER_BTM_6->attachInput(COL_BTM_1);
  PIPE(GUTTER_BTM_5);
  GUTTER_BTM_5->attachInput(GUTTER_BTM_6);
  PIPE(GUTTER_BTM_4);
  GUTTER_BTM_4->attachInput(GUTTER_BTM_5);
  GUTTER_BTM_4->attachInput(COL_BTM_0);
  PIPE(GUTTER_BTM_3);
  GUTTER_BTM_3->attachInput(GUTTER_BTM_4);

  PIPE(GUTTER_BTM_2);
  PIPE(GUTTER_BTM_1);
  GUTTER_BTM_1->attachInput(GUTTER_BTM_6);
  PIPE(GUTTER_BTM_0);
  GUTTER_BTM_0->attachInput(GUTTER_BTM_4);
}
