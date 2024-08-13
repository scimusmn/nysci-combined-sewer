#include "flow.h"
#include <Adafruit_NeoPixel.h>

// (constructor)
Pipe::Pipe(Adafruit_NeoPixel &strip, size_t start, size_t end) 
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
  if (selfFlow != nullptr) {
    // a flow is already started
    return;
  } else {
    selfRate = rate;
    selfFlow = new PipeFlow;
    selfFlow->type = type;
    selfFlow->offset = 0;
    selfFlow->length = rate;
  }
}

// end a self-flow
void Pipe::endFlow() {
  if (selfFlow == nullptr) {
    // no flow exists
    return;
  } else {
    selfFlow->next = flows;
    flows = selfFlow;
    selfFlow = nullptr;
  }
}


// getters for the pipe outputs
unsigned int Pipe::getOutputRate() {
  return outputRate;
}
FlowType Pipe::getOutputType() {
  return outputType;
}


// update the output type/rate from a flow
void Pipe::processFlow(PipeFlow *flow) {
  if (flow == nullptr) { return; }
  if (flow->offset + flow->length > llabs(end - start)) {
    outputRate += speed;
    outputType |= flow->type;
  }
}


// create/update the input flow
void Pipe::updateInput() {
  unsigned int type = FlowType::NO_FLOW;
  unsigned int rate = 0;
  for (PipeSource *source = sources; source != nullptr; source = source->next) {
    rate += source->pipe->getOutputRate();
    type |= source->pipe->getOutputType();
  }

  if (inputFlow != nullptr) {
    if (inputFlow->type == type) {
      // extend existing flow
      inputFlow->length += rate;
    } else {
      // move existing input flow to this->flows
      inputFlow->offset += rate;
      inputFlow->next = flows;
      flows = inputFlow;
      // create new input flow
      inputFlow = new PipeFlow;
      inputFlow->type = type;
      inputFlow->offset = 0;
      inputFlow->length = rate;
    }
  } else {
    // create new input flow
    inputFlow = new PipeFlow;
    inputFlow->type = type;
    inputFlow->offset = 0;
    inputFlow->length = rate;
  }
}


// update the self flow
void Pipe::updateSelf() {
  if (selfFlow != nullptr) {
    selfFlow->length += selfRate;
  }
}


// update all flows and outputs
void Pipe::update() {
  outputRate = 0;
  outputType = NO_FLOW;

  // input flow
  updateInput();
  processFlow(inputFlow);

  // self flow
  updateSelf();
  processFlow(selfFlow);

  // internal flows
  PipeFlow *flow = flows;
  PipeFlow **prev = &flows;
  while (flow != nullptr) {
    flow->offset += 1;
    processFlow(flow);
    if (flow->offset > llabs(end - start)) {
      // remove flow
      *prev = flow->next;
      PipeFlow *next = flow->next;
      delete flow;
      flow = next;
    } else {
      // move on
      prev = &(flow->next);
      flow = flow->next;
    }
  }
}


void drawPixel(Adafruit_NeoPixel &strip, int index, int type) {
  byte r = 0;
  byte g = 0;
  byte b = 0;
  if (type & FlowType::TOILET) {
    r = 255;
  }
  if (type & FlowType::WASHER) {
    g = 255;
  }
  if (type & FlowType::DISHWASHER) {
    b = 255;
  }
  if (type & FlowType::SHOWER) {
    r += 128;
    g += 128;
    b += 128;
  }
  strip.setPixelColor(index, r, g, b);
}

void drawFlow(Adafruit_NeoPixel &strip, int x0, int x1, int step, PipeFlow *flow) {
  if (flow == nullptr) { return; }
  for (int i=0; i<flow->length; i++) {
    int move = step * (i + flow->offset);
    if (x0+move < x1) {
      drawPixel(strip, x0+move, flow->type);
    }
  }
}


// render the pipe flows (to memory)
void Pipe::render() {
  unsigned int step = end > start ? 1 : -1;
  unsigned int x0 = end > start ? start : end;
  unsigned int x1 = end > start ? end : start;
  for (PipeFlow *flow = flows; flow != nullptr; flow = flow->next) {
    drawFlow(strip, x0, x1, step, flow);
  }

  drawFlow(strip, x0, x1, step, inputFlow);
  drawFlow(strip, x0, x1, step, selfFlow);
}
