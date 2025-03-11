#include "flow.h"


#define SEGMENT0 1168, 1208
#define SEGMENT1 1160, 1168
#define SEGMENT2 1208, 1231
#define SEGMENT3 1302, 1353
#define SEGMENT4 8, 49
#define SEGMENT5 0, 8
#define SEGMENT6 50, 72
#define SEGMENT7 142, 193
#define SEGMENT8 1239, 1280
#define SEGMENT9 1231, 1239
#define SEGMENT10 1280, 1302

#define SEGMENT11 1353, 1435
#define SEGMENT12 80, 120
#define SEGMENT13 72, 80
#define SEGMENT14 120, 142
#define SEGMENT15 193, 222
#define SEGMENT16 290, 344
#define SEGMENT17 1458, 1499
#define SEGMENT18 1450, 1458
#define SEGMENT19 1499, 1520
#define SEGMENT20 1520, 1553

#define SEGMENT21 1552, 1594
#define SEGMENT22 222, 259
#define SEGMENT23 344, 395
#define SEGMENT24 1594, 1658
#define SEGMENT25 1658, 1691
#define SEGMENT260 1691, 1720
#define SEGMENT261 1742, 1792
#define SEGMENT27 440, 480
#define SEGMENT28 432, 440
#define SEGMENT29 480, 504
#define SEGMENT30 396, 432

#define SEGMENT31 580, 630
#define SEGMENT32 1871, 1912
#define SEGMENT33 1863, 1871
#define SEGMENT34 1912, 1935
#define SEGMENT35 1792, 1863
#define SEGMENT36 512, 553
#define SEGMENT37 504, 512
#define SEGMENT38 553, 576
#define SEGMENT39 630, 706
#define SEGMENT40 1943, 1984

#define SEGMENT41 1935, 1943
#define SEGMENT42 1984, 2007
#define SEGMENT43 795, 829
#define SEGMENT440 831,833
#define SEGMENT441 833,837
#define SEGMENT442 925, 974
#define SEGMENT45 731, 772
#define SEGMENT46 723, 731
#define SEGMENT47 772, 795
#define SEGMENT480 706, 719
#define SEGMENT481 719, 723
#define SEGMENT49 870, 920
#define SEGMENT50 920, 925


PipeSource * pushPipe(Pipe *pipe, PipeSource *list) {
  PipeSource *node = new PipeSource;
  node->pipe = pipe;
  node->next = list;
  return node;
}


void createPipes(OctoWS2811 &strip, PipeCollections *pipes) {
    TPIPE(0, toilets);
    pipe0->setActivationLevel(2);
    TPIPE(1, showers);
    pipe1->setActivationLevel(2);
    PIPE(2);
    pipe2->attachInput(pipe0);
    pipe2->attachInput(pipe1);
    PIPE(3);
    pipe3->attachInput(pipe2);

    TPIPE(4, showers);
    pipe4->setActivationLevel(3);
    TPIPE(5, toilets);
    pipe5->setActivationLevel(1);
    PIPE(6);
    pipe6->attachInput(pipe4);
    pipe6->attachInput(pipe5);
    PIPE(7);
    pipe7->attachInput(pipe6);

    TPIPE(8, washers);
    pipe8->setActivationLevel(1);
    TPIPE(9, dishwashers);
    pipe9->setActivationLevel(3);
    PIPE(10);
    pipe10->attachInput(pipe8);
    pipe10->attachInput(pipe9);
    PIPE(11);
    pipe11->attachInput(pipe10);
    pipe11->attachInput(pipe3);

    TPIPE(12, showers);
    pipe12->setActivationLevel(1);
    TPIPE(13, toilets);
    pipe13->setActivationLevel(3);
    PIPE(14);
    pipe14->attachInput(pipe12);
    pipe14->attachInput(pipe13);
    PIPE(15);
    pipe15->attachInput(pipe14);
    pipe15->attachInput(pipe7);
    PIPE(16);
    pipe16->attachInput(pipe15);

    TPIPE(17, toilets);
    pipe17->setActivationLevel(1);
    TPIPE(18, showers);
    pipe18->setActivationLevel(3);
    PIPE(19);
    pipe19->attachInput(pipe17);
    pipe19->attachInput(pipe18);
    TPIPE(20, washers);
    pipe20->setActivationLevel(3);
    PIPE(21);
    pipe21->attachInput(pipe19);
    pipe21->attachInput(pipe20);
    pipe21->attachInput(pipe11);

    TPIPE(22, dishwashers);
    pipe22->setActivationLevel(2);
    PIPE(23);
    pipe23->attachInput(pipe22);
    pipe23->attachInput(pipe16);

    TPIPE(24, dishwashers);
    pipe24->setActivationLevel(1);
    TPIPE(25, washers);
    pipe25->setActivationLevel(3);
    PIPE(260);
    pipe260->attachInput(pipe24);
    pipe260->attachInput(pipe25);
    pipe260->attachInput(pipe21);
    PIPE(261);
    pipe261->attachInput(pipe260);

    TPIPE(27, showers);
    pipe27->setActivationLevel(3);
    TPIPE(28, toilets);
    pipe28->setActivationLevel(2);
    PIPE(29);
    pipe29->attachInput(pipe27);
    pipe29->attachInput(pipe28);
    PIPE(30);
    pipe30->attachInput(pipe29);
    pipe30->attachInput(pipe23);
    PIPE(31);
    pipe31->attachInput(pipe30);

    TPIPE(32, dishwashers);
    pipe32->setActivationLevel(3);
    TPIPE(33, washers);
    pipe33->setActivationLevel(3);
    PIPE(34);
    pipe34->attachInput(pipe32);
    pipe34->attachInput(pipe33);
    PIPE(35);
    pipe35->attachInput(pipe34);
    pipe35->attachInput(pipe261);
    pipe35->setOverflowSpeed(0);

    TPIPE(36, toilets);
    pipe36->setActivationLevel(3);
    TPIPE(37, showers);
    pipe37->setActivationLevel(2);
    PIPE(38);
    pipe38->attachInput(pipe36);
    pipe38->attachInput(pipe37);
    PIPE(39);
    pipe39->attachInput(pipe38);
    pipe39->attachInput(pipe31);

    TPIPE(40, washers);
    pipe40->setActivationLevel(1);
    TPIPE(41, washers);
    pipe41->setActivationLevel(3);
    PIPE(42);
    pipe42->attachInput(pipe40);
    pipe42->attachInput(pipe41);
    pipe42->setOverflowSpeed(0);
    TPIPE(43, washers);
    pipe43->setActivationLevel(1);
    PIPE(440);
    pipe440->attachInput(pipe42);
    pipe440->attachInput(pipe43);
    pipe440->attachInput(pipe35);
    pipe440->setOverflowSpeed(0);
    PIPE(441);
    pipe441->attachInput(pipe440);
    PIPE(442);
    pipe442->attachInput(pipe441);
    pipe442->setAsOutput();

    TPIPE(45, washers);
    pipe45->setActivationLevel(2);
    TPIPE(46, washers);
    pipe46->setActivationLevel(1);
    PIPE(47);
    pipe47->attachInput(pipe45);
    pipe47->attachInput(pipe46);
    pipe47->setOverflowSpeed(0);
    PIPE(480);
    pipe480->setOverflowSpeed(0);
    pipe480->attachInput(pipe47);
    pipe480->attachInput(pipe39);
    PIPE(481);
    pipe481->attachInput(pipe480);
    PIPE(49);
    pipe49->attachInput(pipe481);
    pipe49->setAsOutput();
    // PIPE(50);
    // pipe50->attachInput(pipe49);

}
