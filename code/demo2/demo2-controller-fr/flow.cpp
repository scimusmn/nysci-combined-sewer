#include "flow.h"
#include <OctoWS2811.h>

// (constructor)
Pipe::Pipe(int pipeId, OctoWS2811 &strip, size_t start, size_t end) 
  : pipeId(pipeId), strip(strip), start(start), end(end) {
  for (int i=0; i<N_INPUTS; i++) {
    sources[i] = nullptr;
  }
  for(int i=0; i<N_FLOWS; i++) {
    movingFlows[i].active = false;
  }
  inputFlow.active = false;
}


// attach another pipe as an input
void Pipe::attachInput(Pipe *pipe) {
  for (int i=0; i<N_INPUTS; i++) {
    if (sources[i] == nullptr) {
      sources[i] = pipe;
      return;
    }
  }
  Serial.print("!! WARNING !! no space left to attach input on pipe ");
  Serial.println(pipeId);
}


// create a self-flow
void Pipe::startFlow() {
  isFlowing = true;
}

// end a self-flow
void Pipe::endFlow() {
  isFlowing = false;
}


unsigned int Pipe::outputCount() {
  return outputFlowing;
}


// update the output type/rate from a flow
void Pipe::processFlow(PipeFlow &flow) {
  if (flow.active) {
    if (flow.offset + flow.length >= llabs(end - start)) {
      outputFlowing += flow.count;
    }
  }
}


unsigned int countInputFlows(Pipe **sources) {
  unsigned int count = 0;
  for (int i=0; i<N_INPUTS; i++) {
    if (sources[i] == nullptr) {
      break;
    } else {
      count += sources[i]->outputCount();
    }
  }
  return count;
}


void Pipe::convertInputToMovingFlow() {
  if (inputFlow.active) {
    insertFlow(inputFlow);
    inputFlow.active = false;
  }
}


void Pipe::insertFlow(PipeFlow f) {
  for (int i=0; i<N_FLOWS; i++) {
    if (!movingFlows[i].active) {
      movingFlows[i] = f;
      return;
    }
  }
  Serial.print("!! WARNING !! no space left to insert flow on pipe ");
  Serial.println(pipeId);
}


// create/update the input flow
void Pipe::updateInput() {
  unsigned int flowCount = countInputFlows(sources);
  if (isFlowing) {
    flowCount += 1;
  }

  // if (flowCount != this->flowCount) {
  //   Serial.print(this->flowCount); Serial.print(" -> "); Serial.println(flowCount);
  // }


  if (flowCount == 0) {
    // move any existing input flow to this->flows
    convertInputToMovingFlow();
  } else {
    // add a new flow if count has increased;
    if (flowCount != this->flowCount) {
      // Serial.print("adding new flow "); Serial.println(flowCount);
      convertInputToMovingFlow();
      inputFlow.offset = 0;
      inputFlow.length = speed;
      inputFlow.count = flowCount;
      inputFlow.gradient = flowCount > this->flowCount;
      inputFlow.active = true;
    } else {
      if (inputFlow.active) {
        inputFlow.length += 1;
      }
    }
  }
  this->flowCount = flowCount;
}


// update all flows and outputs
void Pipe::update() {
  // bool outputWasFlowing = outputFlowing;
  outputFlowing = 0;

  // input flow
  updateInput();
  processFlow(inputFlow);

  // internal flows
  for (int i=0; i<N_FLOWS; i++) {
    PipeFlow &flow = movingFlows[i];
    if (flow.active) {
      flow.offset += 1;
      if (flow.offset >= llabs(end - start)) {
        flow.active = false;
      } else {
        processFlow(flow);
      }
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


void drawPixel(OctoWS2811 &strip, int index, float alpha) {
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

void Pipe::drawFlow(PipeFlow &flow) {
  if (!flow.active) { return; }
  for (int i=0; i<flow.length; i++) {
    int idx = i + flow.offset;
    if (idx < length()) {
      double x = flow.length - i;
      x -= 20;
      double alpha = x < 0 ? 1.0 : exp(-x/20);
      if (!flow.gradient) {
        alpha = 0;
      }
      //double alpha = 1.0;
      drawPixel(strip, stripIndex(idx), alpha);
    }
  }
}


// render the pipe flows (to memory)
void Pipe::render() {
  for (int i = 0; i != length(); i++) {
    strip.setPixel(stripIndex(i), 0);
    // drawBg(strip, i);
  }
  for (int i=0; i<N_FLOWS; i++) {
    drawFlow(movingFlows[i]);
  }

  drawFlow(inputFlow);
}


unsigned int Pipe::stripIndex(unsigned int i) {
  if (start < end) {
    return start+i;
  } else {
    return start-i;
  }
}


unsigned int Pipe::length() {
  if (start < end) {
    return end - start;
  } else {
    return start - end;
  }
}
