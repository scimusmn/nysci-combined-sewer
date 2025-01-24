#include "flow.h"
#include "messages.h"
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
VirtualPipe::VirtualPipe(OctoWS2811 &strip, unsigned int length)
  : Pipe(-1, strip, 0, 0), len(length) {}


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

void Pipe::attachCanInput(uint8_t node, unsigned int pipeId) {
  useCanInput = true;
  canInput.node = node;
  canInput.pipeId = pipeId;
  canInput.overflowing = false;
}


// set whether the pipe should output CAN PipeOutput messages
void Pipe::setAsOutput(bool out) {
  canOutput = out;
}


void Pipe::setOverflowThreshold(unsigned int thresh) {
  overflowThreshold = thresh;
}


void Pipe::setOutputOverflowing(bool overflow) {
  outputOverflowing = overflow;
}


// create a self-flow
void Pipe::startFlow(unsigned int count) {
  selfInputCount = count;
}

// end a self-flow
void Pipe::endFlow() {
  selfInputCount = 0;
}


unsigned int Pipe::getOutputCount() {
  return outputCount;
}


// update the output type/rate from a flow
void Pipe::processFlow(PipeFlow &flow) {
  if (flow.active) {
    if (flow.offset + flow.length >= length()) {
      outputCount += flow.count;
    }
  }
}


unsigned int countInputFlows(Pipe **sources) {
  unsigned int count = 0;
  for (int i=0; i<N_INPUTS; i++) {
    if (sources[i] == nullptr) {
      break;
    } else {
      count += sources[i]->getOutputCount();
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


// add a new moving flow
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


void Pipe::updateCanInput(uint8_t srcId, PipeOutput output) {
  if (!useCanInput) { return; }
  if ((srcId == canInput.node) && (output.pipeId == canInput.pipeId)) {
    canInputFlow = output.count;
  }
}

void Pipe::updateCanOverflow(PipeOverflow o) {
  if (!canOutput) { return; }
  if ((o.node == selfNodeId()) && (o.pipeId == pipeId)) {
    outputOverflowing = o.overflowing;
  }
}




// create/update the input flow
void Pipe::updateInput() {
  unsigned int flowCount = selfInputCount + canInputFlow + countInputFlows(sources);

  // if (flowCount != this->flowCount) {
  //   Serial.print(this->flowCount); Serial.print(" -> "); Serial.println(flowCount);
  // }


  if (flowCount == 0) {
    // move any existing input flow to this->flows
    convertInputToMovingFlow();
  } else {
    // add a new flow if count has increased;
    if (flowCount != this->totalInputCount) {
      // Serial.print("adding new flow "); Serial.println(flowCount);
      unsigned int gradOff = inputFlow.length + inputFlow.gradientOffset;
      convertInputToMovingFlow();
      inputFlow.offset = 0;
      inputFlow.length = speed;
      inputFlow.count = flowCount;
      if (flowCount > this->totalInputCount) {
        // new input flow, show gradient
        inputFlow.gradientOffset = 0;
      } else {
        inputFlow.gradientOffset = gradOff;
      }
      inputFlow.active = true;
    } else {
      if (inputFlow.active) {
        if (inputFlow.length < length()) {
          inputFlow.length += speed;
        } else if (inputFlow.gradientOffset < 1024) {
          inputFlow.gradientOffset += speed;
        }
      }
    }
  }
  this->totalInputCount = flowCount;
}


// update all flows and outputs
void Pipe::update() {
  unsigned int oldOutputCount = outputCount;
  outputCount = 0;

  // input flow
  updateInput();
  processFlow(inputFlow);

  // internal flows
  for (int i=0; i<N_FLOWS; i++) {
    PipeFlow &flow = movingFlows[i];
    if (flow.active) {
      flow.offset += speed;
      if (flow.offset >= length()) {
        flow.active = false;
      } else {
        processFlow(flow);
      }
    }
  }

  if ((outputCount != oldOutputCount) && canOutput) {
    sendCanBusPipeOutput({ static_cast<unsigned int>(pipeId), outputCount });
  }

  updateOverflow();
}


bool Pipe::isOverflowed() {
  return overflowLevel > 0;
}



void Pipe::updateOverflow() {
  if (outputOverflowing || (overflowThreshold && (outputCount > overflowThreshold))) {
    if (overflowLevel < length()) {
      overflowLevel += overflowRate;
    }
  } else { // no longer overflowing!
    bool inputsOverflowed = false;
    for (int i=0; i<N_INPUTS; i++) {
      if (sources[i] == nullptr) { break; }
      else {
        inputsOverflowed = inputsOverflowed || sources[i]->isOverflowed();
      }
    }
    if (!inputsOverflowed) {
      if (overflowLevel > 1) {
        overflowLevel -= overflowRate;
      } else {
        overflowLevel = 0;
      }
    }
  }

  Serial.print(pipeId); Serial.print(": "); Serial.println(overflowLevel);

  // update inputs
  if (inputOverflowing != (overflowLevel > length())) {
    inputOverflowing = overflowLevel > length();

    for (int i=0; i<N_INPUTS; i++) {
      if (sources[i] == nullptr) {
        break;
      } else {
        sources[i]->setOutputOverflowing(inputOverflowing);
      }
    }
  
    if (useCanInput) {
      Serial.print("send can overflow on "); Serial.print(canInput.node); Serial.print(":"); Serial.println(canInput.pipeId);
      canInput.overflowing = inputOverflowing;
      sendCanBusPipeOverflow(canInput);
    }
  }
} 


/* ================================================================ *\
 *                                                                  *
 *                            RENDERING                             *
 *                                                                  *
 * ================================================================ */

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
  return { 
    0, 
    static_cast<uint8_t>(128 * level),
    static_cast<uint8_t>((200 * level) + 32)
  };
  // return { 0, 0, 0 };
}

color_t leaderColor(int index) {
  return { 0, 255, 128 };
}


void drawBg(OctoWS2811 &strip, int index) {
  color_t c = bgColor(index);
  strip.setPixel(index, c.r, c.g, c.b);
}


void drawPixel(OctoWS2811 &strip, int index, float alpha) {
  color_t c = alphaBlend(bgColor(index), leaderColor(index), alpha);
  strip.setPixel(index, c.r, c.g, c.b);
}

void Pipe::drawFlow(PipeFlow &flow) {
  if (!flow.active) { return; }
  for (unsigned int i=0; i<flow.length; i++) {
    unsigned int idx = i + flow.offset;
    if (idx < length()) {
      double x = flow.length + flow.gradientOffset - i;
      x -= 20;
      double alpha = x < 0 ? 1.0 : exp(-x/20);
      //double alpha = 1.0;
      drawPixel(strip, stripIndex(idx), alpha);
    }
  }
}


// render the pipe flows (to memory)
void Pipe::render() {
  for (unsigned int i = 0; i != length(); i++) {
    strip.setPixel(stripIndex(i), 0);
    // drawBg(strip, i);
  }
  for (int i=0; i<N_FLOWS; i++) {
    drawFlow(movingFlows[i]);
  }

  drawFlow(inputFlow);

  if (overflowLevel != 0) {
    for (int i=0; i<static_cast<int>(fmin(overflowLevel, length())); i++) {
      unsigned int idx = length() - i - 1;
      color_t c = leaderColor(idx);
      strip.setPixel(stripIndex(idx), c.r, c.g, c.b);
    }
  }
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


unsigned int VirtualPipe::length() {
  return len;
}
void VirtualPipe::render() {}
