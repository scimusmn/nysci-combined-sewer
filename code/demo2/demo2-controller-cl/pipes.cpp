#include "flow.h"


#define SEGMENT0 1128, 1170
#define SEGMENT1 1120, 1128
#define SEGMENT2 1170, 1191
#define SEGMENT3 
#define SEGMENT4 
#define SEGMENT5 
#define SEGMENT6 
#define SEGMENT7 
#define SEGMENT8 
#define SEGMENT9 
#define SEGMENT10 
#define SEGMENT11 
#define SEGMENT12 
#define SEGMENT13 


#define SEGMENT4 8, 47
#define SEGMENT5 0, 8
#define SEGMENT6 47, 72


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
  #define STARTEND_(number) SEGMENT ## number
  #define STARTEND(number) STARTEND_(number)
  #define PIPE(number) \
    Pipe * pipe ## number = new Pipe(strip, STARTEND(number)); \
    *pipes = pushPipe(pipe ## number, *pipes);
  #define TPIPE(number, type) \
    PIPE(number) \
    *type = pushPipe(pipe ## number, *type); \
  

}
