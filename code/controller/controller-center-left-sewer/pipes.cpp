#include "flow.h"

#define SEGMENT1 0, 72 //  
#define SEGMENT2 149, 72 // 
#define SEGMENT3 292, 293 // 
#define SEGMENT4 293, 418 // 
#define SEGMENT5 483, 418 //
#define SEGMENT6 546, 483 // 

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
  TPIPE(1, constant);
  TPIPE(2,constant);

  TPIPE(3, constant);
  PIPE(4);
  pipe4->attachInput(pipe3);

  TPIPE(6,constant);
  PIPE(5);
  pipe5->attachInput(pipe6);
}
