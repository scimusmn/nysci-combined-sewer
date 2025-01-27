#include "flow.h"


#define SEGMENT0 1168, 1208 //
#define SEGMENT10 1160, 1168 //
#define SEGMENT20 1208, 1231 //
#define SEGMENT30 1302, 1353 //
#define SEGMENT40 8, 49 //
#define SEGMENT50 0, 8 //
#define SEGMENT60 50, 72 //
#define SEGMENT70 142, 193 //
#define SEGMENT80 1239, 1280 //
#define SEGMENT90 1231, 1239 //
#define SEGMENT100 1280, 1302 //

#define SEGMENT110 1353, 1435 //
#define SEGMENT120 80, 120 //
#define SEGMENT130 72, 80 //
#define SEGMENT140 120, 142 //
#define SEGMENT150 193, 222 //
#define SEGMENT160 290, 344 //
#define SEGMENT170 1458, 1499 //
#define SEGMENT180 1450, 1458 //
#define SEGMENT190 1499, 1520 //
#define SEGMENT200 1520, 1553

#define SEGMENT210 1552, 1594
#define SEGMENT220 222, 259
#define SEGMENT230 344, 395
#define SEGMENT240 1594, 1658
#define SEGMENT250 1658, 1691

#define SEGMENT260 1691, 1720
#define SEGMENT265 1742, 1792

#define SEGMENT270 440, 480
#define SEGMENT280 432, 440
#define SEGMENT290 480, 504
#define SEGMENT300 396, 432

#define SEGMENT310 580, 630
#define SEGMENT320 1871, 1912
#define SEGMENT330 1863, 1871
#define SEGMENT340 1912, 1935
#define SEGMENT350 1792, 1863
#define SEGMENT360 512, 553
#define SEGMENT370 504, 512
#define SEGMENT380 553, 576
#define SEGMENT390 630, 706
#define SEGMENT400 1943, 1984

#define SEGMENT410 1935, 1943
#define SEGMENT420 1984, 2007
#define SEGMENT430 795, 829

#define SEGMENT440 829, 837
#define SEGMENT445 925, 974



#define SEGMENT450 731, 772
#define SEGMENT460 723, 731
#define SEGMENT470 772, 795
#define SEGMENT480 706, 723
#define SEGMENT490 870, 920
<<<<<<< HEAD
#define SEGMENT500 925, 920
=======
#define SEGMENT500 920, 925
>>>>>>> FR_Update


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
    TPIPE(0, toilets);
    TPIPE(10, showers);
    PIPE(20);
    pipe20->attachInput(pipe0);
    pipe20->attachInput(pipe10);
    PIPE(30);
    pipe30->attachInput(pipe20);
<<<<<<< HEAD

    TPIPE(40, showers);
    TPIPE(50, toilets);
    PIPE(60);
    pipe60->attachInput(pipe40);
    pipe60->attachInput(pipe50);
    PIPE(70);
    pipe70->attachInput(pipe60);

    TPIPE(80, washers);
    TPIPE(90, dishwashers);
    PIPE(100);
    pipe100->attachInput(pipe80);
    pipe100->attachInput(pipe90);
    PIPE(110);
    pipe110->attachInput(pipe100);
    pipe110->attachInput(pipe30);

    TPIPE(120, showers);
    TPIPE(130, toilets);
    PIPE(140);
    pipe140->attachInput(pipe120);
    pipe140->attachInput(pipe130);
    PIPE(150);
    pipe150->attachInput(pipe140);
    pipe150->attachInput(pipe70);
    PIPE(160);
    pipe160->attachInput(pipe150);

    TPIPE(170, toilets);
    TPIPE(180, showers);
    PIPE(190);
    pipe190->attachInput(pipe170);
    pipe190->attachInput(pipe190);
    TPIPE(200, washers);
    PIPE(210);
    pipe210->attachInput(pipe190);
    pipe210->attachInput(pipe200);
    pipe210->attachInput(pipe110);

    TPIPE(220, dishwashers);
    PIPE(230);
    pipe230->attachInput(pipe220);
    pipe230->attachInput(pipe160);

    TPIPE(240, dishwashers);
    TPIPE(250, washers);
    PIPE(260);
    pipe260->attachInput(pipe240);
    pipe260->attachInput(pipe250);
    pipe260->attachInput(pipe210);

    PIPE(265);
    pipe265->attachInput(pipe260);

    TPIPE(270, showers);
    TPIPE(280, toilets);
    PIPE(290);
    pipe290->attachInput(pipe270);
    pipe290->attachInput(pipe280);
    PIPE(300);
    pipe300->attachInput(pipe290);
    pipe300->attachInput(pipe230);
    PIPE(310);
    pipe310->attachInput(pipe300);

    TPIPE(320, dishwashers);
    TPIPE(330, washers);
    PIPE(340);
    pipe340->attachInput(pipe320);
    pipe340->attachInput(pipe330);
    PIPE(350);
    pipe350->attachInput(pipe340);
    pipe350->attachInput(pipe265);

    TPIPE(360, toilets);
    TPIPE(370, showers);
    PIPE(380);
    pipe380->attachInput(pipe360);
    pipe380->attachInput(pipe370);
    PIPE(390);
    pipe390->attachInput(pipe380);
    pipe390->attachInput(pipe310);

    TPIPE(400, washers);
    TPIPE(410, washers);
    PIPE(420);
    pipe420->attachInput(pipe400);
    pipe420->attachInput(pipe410);
    TPIPE(430, washers);
    PIPE(440);
    pipe440->attachInput(pipe420);
    pipe440->attachInput(pipe430);
    pipe440->attachInput(pipe350);

    PIPE(445);
    pipe445->attachInput(pipe440);

    TPIPE(450, washers);
    TPIPE(460, washers);
    PIPE(470);
    pipe470->attachInput(pipe450);
    pipe470->attachInput(pipe460);
    PIPE(480);
    pipe480->attachInput(pipe470);
    pipe480->attachInput(pipe390);
    PIPE(490);
    pipe490->attachInput(pipe480);
    TPIPE(500, constant);
    //pipe500->attachInput(pipe490);

    //TPIPE()
=======
>>>>>>> FR_Update

    TPIPE(40, showers);
    TPIPE(50, toilets);
    PIPE(60);
    pipe60->attachInput(pipe40);
    pipe60->attachInput(pipe50);
    PIPE(70);
    pipe70->attachInput(pipe60);

    TPIPE(80, washers);
    TPIPE(90, dishwashers);
    PIPE(100);
    pipe100->attachInput(pipe80);
    pipe100->attachInput(pipe90);
    PIPE(110);
    pipe110->attachInput(pipe100);
    pipe110->attachInput(pipe30);

    TPIPE(120, showers);
    TPIPE(130, toilets);
    PIPE(140);
    pipe140->attachInput(pipe120);
    pipe140->attachInput(pipe130);
    PIPE(150);
    pipe150->attachInput(pipe140);
    pipe150->attachInput(pipe70);
    PIPE(160);
    pipe160->attachInput(pipe150);

    TPIPE(170, toilets);
    TPIPE(180, showers);
    PIPE(190);
    pipe190->attachInput(pipe170);
    pipe190->attachInput(pipe190);
    TPIPE(200, washers);
    PIPE(210);
    pipe210->attachInput(pipe190);
    pipe210->attachInput(pipe200);
    pipe210->attachInput(pipe110);

    TPIPE(220, dishwashers);
    PIPE(230);
    pipe230->attachInput(pipe220);
    pipe230->attachInput(pipe160);

    TPIPE(240, dishwashers);
    TPIPE(250, washers);
    PIPE(260);
    pipe260->attachInput(pipe240);
    pipe260->attachInput(pipe250);
    pipe260->attachInput(pipe210);

    PIPE(265);
    pipe265->attachInput(pipe260);

    TPIPE(270, showers);
    TPIPE(280, toilets);
    PIPE(290);
    pipe290->attachInput(pipe270);
    pipe290->attachInput(pipe280);
    PIPE(300);
    pipe300->attachInput(pipe290);
    pipe300->attachInput(pipe230);
    PIPE(310);
    pipe310->attachInput(pipe300);

    TPIPE(320, dishwashers);
    TPIPE(330, washers);
    PIPE(340);
    pipe340->attachInput(pipe320);
    pipe340->attachInput(pipe330);
    PIPE(350);
    pipe350->attachInput(pipe340);
    pipe350->attachInput(pipe265);

    TPIPE(360, toilets);
    TPIPE(370, showers);
    PIPE(380);
    pipe380->attachInput(pipe360);
    pipe380->attachInput(pipe370);
    PIPE(390);
    pipe390->attachInput(pipe380);
    pipe390->attachInput(pipe310);

    TPIPE(400, washers);
    TPIPE(410, washers);
    PIPE(420);
    pipe420->attachInput(pipe400);
    pipe420->attachInput(pipe410);
    TPIPE(430, washers);
    PIPE(440);
    pipe440->attachInput(pipe420);
    pipe440->attachInput(pipe430);
    pipe440->attachInput(pipe350);

    PIPE(445);
    pipe445->attachInput(pipe440);

    TPIPE(450, washers);
    TPIPE(460, washers);
    PIPE(470);
    pipe470->attachInput(pipe450);
    pipe470->attachInput(pipe460);
    PIPE(480);
    pipe480->attachInput(pipe470);
    pipe480->attachInput(pipe390);
    PIPE(490);
    pipe480->attachInput(pipe480);
    PIPE(500);
    pipe500->attachInput(pipe490);

}