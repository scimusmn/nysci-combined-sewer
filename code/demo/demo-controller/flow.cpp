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
  selfType = type;
}

// end a self-flow
void Pipe::endFlow() {
  selfType = NO_FLOW;
}


// getters for the pipe outputs
unsigned int Pipe::getOutputRate() {
  return outputRate;
}
unsigned int Pipe::getOutputType() {
  return outputType;
}


// update the output type/rate from a flow
void Pipe::processFlow(PipeFlow *flow) {
  if (flow == nullptr) { return; }
  if (flow->offset + flow->length >= llabs(end - start)) {
    outputRate = speed;
    outputType |= flow->type;
  }
}


// create/update the input flow
void Pipe::updateInput() {
  unsigned int type = selfType;
  unsigned int rate = 0;
  for (PipeSource *source = sources; source != nullptr; source = source->next) {
    rate = source->pipe->getOutputRate();
    type |= source->pipe->getOutputType();
  }

  if (type == FlowType::NO_FLOW) {
    if (inputFlow != nullptr) {
      // move existing input flow to this->flows
      // inputFlow->offset += speed;
      inputFlow->next = flows;
      flows = inputFlow;
      inputFlow = nullptr;
    }
  } else {
    if (inputFlow != nullptr) {
      if (inputFlow->type == type) {
        // extend existing flow
        inputFlow->length += speed;
      } else {
        // move existing input flow to this->flows
        // inputFlow->offset += speed;
        inputFlow->next = flows;
        flows = inputFlow;
        // create new input flow
        inputFlow = new PipeFlow;
        inputFlow->type = type;
        inputFlow->offset = 0;
        inputFlow->length = speed;
      }
    } else {
      // create new input flow
      inputFlow = new PipeFlow;
      inputFlow->type = type;
      inputFlow->offset = 0;
      inputFlow->length = speed;
    }
  }
}


// update all flows and outputs
void Pipe::update() {
  outputRate = 0;
  outputType = NO_FLOW;

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
  color_t c;
  c.r = ulerp(a.r, b.r, alpha);
  c.g = ulerp(a.g, b.g, alpha);
  c.b = ulerp(a.b, b.b, alpha);
  return c;
}


color_t bgColor(int index) {
  double x = ((double)index) / 30.0;
  double t = ((double)millis()) / 1000.0;
  double level = 0.5 + (0.5 * sin(10*(x - t)));
  return { 0, 2, 8 * level + 3 };
  // return { 0, 0, 0 };
}


void drawBg(OctoWS2811 &strip, int index) {
  color_t c = bgColor(index);
  strip.setPixel(index, c.r, c.g, c.b);
}


void drawPixel(OctoWS2811 &strip, int index, int type, float alpha) {
  color_t c;
  if (type & FlowType::TOILET) {
    c = alphaBlend(c, { 64, 0, 0 }, 0.5);
  }
  if (type & FlowType::WASHER) {
    c = alphaBlend(c, { 0, 64, 0 }, 0.5);
  }
  if (type & FlowType::DISHWASHER) {
    c = alphaBlend(c, { 0, 0, 128 }, 0.5);
  }
  if (type & FlowType::SHOWER) {
    c = alphaBlend(c, { 32, 32, 0 }, 0.5);
  }
  c = alphaBlend(bgColor(index), c, alpha);
  strip.setPixel(index, c.r, c.g, c.b);
}

void drawFlow(OctoWS2811 &strip, int x0, int x1, int step, PipeFlow *flow) {
  if (flow == nullptr) { return; }
  for (int i=0; i<flow->length; i++) {
    int move = step * (i + flow->offset);
    if (x0+move < x1) {
      double x = flow->length - i;
      double alpha = exp(-x/3);
      drawPixel(strip, x0+move, flow->type, 0.8 * alpha + 0.2);
    }
  }
}


// render the pipe flows (to memory)
void Pipe::render() {
  unsigned int step = end > start ? 1 : -1;
  unsigned int x0 = end > start ? start : end;
  unsigned int x1 = end > start ? end : start;
  for (int i = x0; i != x1; i += step) {
    drawBg(strip, i);
  }
  for (PipeFlow *flow = flows; flow != nullptr; flow = flow->next) {
    drawFlow(strip, x0, x1, step, flow);
  }

  drawFlow(strip, x0, x1, step, inputFlow);
}
