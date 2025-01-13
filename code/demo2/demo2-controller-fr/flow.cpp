#include "flow.h"
#include <OctoWS2811.h>

// (constructor)
Pipe::Pipe(OctoWS2811 &strip, size_t start, size_t end) 
  : strip(strip), start(start), end(end) {
}


// attach another pipe as an input
void Pipe::attachInput(Pipe *pipe) {
  struct PipeSource *source = new PipeSource;
  source->pipe = pipe;
  source->next = sources;
  sources = source;
}


// create a self-flow
void Pipe::startFlow(FlowType type, unsigned int rate) {
  isFlowing = true;
}

// end a self-flow
void Pipe::endFlow() {
  isFlowing = false;
}



unsigned int Pipe::outputIsFlowing() {
  return outputFlowing;
}


// update the output type/rate from a flow
void Pipe::processFlow(PipeFlow *flow) {
  if (flow == nullptr) { return; }
  if (flow->offset + flow->length >= llabs(end - start)) {
    outputFlowing = true;
  }
}


unsigned int countInputFlows(PipeSource *sources, unsigned long *time) {
  unsigned int flowCount = 0;
  for (PipeSource *source = sources; source != nullptr; source = source->next) {
    if (
      source->pipe->outputIsFlowing()
    ) {
      flowCount += 1;
    }
  }
  return flowCount;
}



void Pipe::removeInputFlow() {
  if (inputFlow != nullptr) {
    inputFlow->next = flows;
    flows = inputFlow;
    inputFlow = nullptr;
  }
}


// create/update the input flow
void Pipe::updateInput() {
  unsigned int flowCount = countInputFlows(sources, &time);
  if (isFlowing) {
    flowCount += 1;
  }


  if (flowCount == 0) {
    // move any existing input flow to this->flows
    removeInputFlow();
  } else {
    // add a new flow if count has increased;
    if (flowCount > this->flowCount) {
      removeInputFlow();
      inputFlow = new PipeFlow;
      inputFlow->offset = 0;
      inputFlow->length = speed;
    } else {
      inputFlow->length += speed;
    }

    this->flowCount = flowCount;
  }
}


// update all flows and outputs
void Pipe::update() {
  // bool outputWasFlowing = outputFlowing;
  outputFlowing = false;

  // input flow
  updateInput();
  processFlow(inputFlow);

  // internal flows
  PipeFlow *flow = flows;
  PipeFlow **prev = &flows;
  while (flow != nullptr) {
    flow->offset += 1;
    if (flow->offset >= llabs(end - start)) {
      // remove flow
      *prev = flow->next;
      PipeFlow *next = flow->next;
      delete flow;
      flow = next;
    } else {
      // process & move on
      processFlow(flow);
      prev = &(flow->next);
      flow = flow->next;
    }
  }
}


typedef struct {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
} color_t;


uint8_t ulerp(uint8_t a, uint8_t b, float theta) {
  double aa = a;
  double bb = b;
  double da = (1-theta) * aa;
  double db = (theta) * bb;
  return da + db;
}

color_t alphaBlend(color_t a, color_t b, float alpha) {
  if (a.r == 0 && a.g == 0 && a.b == 0) {
    return b;
  }
  color_t c;
  c.r = ulerp(a.r, b.r, alpha);
  c.g = ulerp(a.g, b.g, alpha);
  c.b = ulerp(a.b, b.b, alpha);
  return c;
}


color_t bgColor(int index) {
  double x = ((double)index) / 30.0;
  double t = ((double)millis()) / 1000.0;
  double level = 0.5 + (0.5 * sin((x - t)));
  return { 0, 128 * level, (200 * level) + 32 };
  // return { 0, 0, 0 };
}


void drawBg(OctoWS2811 &strip, int index) {
  color_t c = bgColor(index);
  strip.setPixel(index, c.r, c.g, c.b);
}


void drawPixel(OctoWS2811 &strip, int index, int type, float alpha) {
  color_t c = { 255, 255, 255 };
  // if (type & FlowType::RAIN) {
  //   c = { 0, 0, 255 };
  // }
  // if (type & FlowType::TOILET) {
  //   c = { 255, 0, 0 };
  //   // c = alphaBlend(c, { 255, 0, 0 }, 0.5);
  // }
  // if (type & FlowType::WASHER) {
  //   c = { 0, 255, 0 };
  //   // c = alphaBlend(c, { 0, 255, 0 }, 0.5);
  // }
  // if (type & FlowType::DISHWASHER) {
  //   c = { 255, 0, 255 };
  //   // c = alphaBlend(c, { 255, 0, 255 }, 0.5);
  // }
  // if (type & FlowType::SHOWER) {
  //   c = { 255, 255, 0 };
  //   // c = alphaBlend(c, { 255, 255, 0 }, 0.5);
  // }
  c = alphaBlend(bgColor(index), c, alpha);
  strip.setPixel(index, c.r, c.g, c.b);
}

void drawFlow(OctoWS2811 &strip, int x0, int x1, int step, PipeFlow *flow) {
  if (flow == nullptr) { return; }
  for (int i=0; i<flow->length; i++) {
    int move = step * (i + flow->offset);
    if (x0+move < x1) {
      double x = flow->length - i;
      x -= 20;
      double alpha = x < 0 ? 1.0 : exp(-x/20);
      //double alpha = 1.0;
      drawPixel(strip, x0+move, flow->type, alpha);
    }
  }
}


// render the pipe flows (to memory)
void Pipe::render() {
  unsigned int step = end > start ? 1 : -1;
  unsigned int x0 = end > start ? start : end;
  unsigned int x1 = end > start ? end : start;
  for (int i = x0; i != x1; i += step) {
    strip.setPixel(i, 0);
    // drawBg(strip, i);
  }
  for (PipeFlow *flow = flows; flow != nullptr; flow = flow->next) {
    drawFlow(strip, x0, x1, step, flow);
  }

  drawFlow(strip, x0, x1, step, inputFlow);
}
