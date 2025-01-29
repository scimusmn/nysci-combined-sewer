#include "flow.h"
#include "messages.h"
#include <OctoWS2811.h>


size_t stripLength(size_t start, size_t end) {
  if (start < end) {
    return end - start;
  } else {
    return start - end;
  }
}


/****************************************************************\
 *                                                              *
 *                         PipeFlow                             *
 *                                                              *
\****************************************************************/


void PipeFlow::advance(unsigned int maxOffset, unsigned int speed) {
  if (!active) { return; }
  offset += speed;
  if (offset >= maxOffset) {
    active = false;
  }
}


void InputFlow::reset(unsigned int count, unsigned int speed) {
  offset = 0;
  length = speed;
  this->count = count;
  gradientOffset = 0;
}


void InputFlow::advance(unsigned int maxLength, unsigned int speed) {
  if (!active) { return; }
  if (this->length < maxLength) {
    this->length += speed;
  } else if (gradientOffset < 1024) {
    gradientOffset += speed;
  }
}



/****************************************************************\
 *                                                              *
 *                         PipeInput                           *
 *                                                              *
\****************************************************************/


// (constructor)
PipeInput::PipeInput() {
  for (int i=0; i<N_INPUTS; i++) {
    localSource[i] = nullptr;
  }
}


// attach a local pipe as an input
void PipeInput::attachInput(Pipe *pipe, unsigned int pipeId) {
  for (int i=0; i<N_INPUTS; i++) {
    if (localSource[i] == nullptr) {
      localSource[i] = pipe;
      return;
    }
  }
  Serial.print("!! WARNING !! no space left to attach input on pipe ");
  Serial.println(pipeId);
}


// attach a remote pipe as input
void PipeInput::attachCanInput(uint8_t node, unsigned int pipeId) {
  useCan = true;
  canNode = node;
  canPipe = pipeId;
}


// process incoming PIPE_OUTPUT can messages
void PipeInput::updateCanInput(uint8_t srcNode, CanPipeOutput output) {
  if (!useCan) { return; }
  if ((srcNode == canNode) && (output.pipeId == canPipe)) {
    canCount = output.count;
  }
}

void PipeInput::updateCanOverflow(uint8_t srcNode, CanPipeOverflow overflow) {
  if (!useCan) { return; }
  if ((srcNode == canNode) && (overflow.pipeId == canPipe)) {
    if (overflow.action == CanPipeOverflow::Action::IS_DRAINED) {
      canDrained = true;
    }
  }
}


// count the total input flows
unsigned int PipeInput::countFlows(unsigned int pipeId) {
  unsigned int localCount = 0;
  for (int i=0; i<N_INPUTS; i++) {
    if (localSource[i] == nullptr) { break; }
    localCount += localSource[i]->getOutputCount();
  }

  unsigned int newCount = selfCount + canCount + localCount;
  changed = (newCount != prevCount);
  increased = (newCount > prevCount);
  prevCount = newCount;
  // Serial.print(pipeId); Serial.print("[in]: "); Serial.println(newCount);
  return newCount;
}


bool PipeInput::countIncreased() {
  return increased;
}

bool PipeInput::countChanged() {
  return changed;
}


void PipeInput::setOverflowing() {
  for (int i=0; i<N_INPUTS; i++) {
    if (localSource[i] == nullptr) { break; }
    localSource[i]->setOverflowing();
  }

  if (useCan) {
    sendCanBusPipeOverflow({ canPipe, canNode, CanPipeOverflow::Action::SET_OVERFLOWING });
    canDrained = false;
  }
}


void PipeInput::setDraining() {
  for (int i=0; i<N_INPUTS; i++) {
    if (localSource[i] == nullptr) { break; }
    localSource[i]->setDraining();
  }

  if (useCan) {
    sendCanBusPipeOverflow({ canPipe, canNode, CanPipeOverflow::Action::SET_DRAINING });
  }
}


bool PipeInput::drained() {
  bool drained = true;
  if (useCan) {
    drained = canDrained;
  }
  int count = 0;
  for (; count<N_INPUTS; count++) {
    if (localSource[count] == nullptr) { break; }
    drained = drained && localSource[count]->drained();
  }

  //if (count == 0) {
  //  if (useCan) {
  //    return canDrained;
  //  } else {
  //    return 
  return drained;
}


bool PipeInput::isOverflowing() {
  bool overflowing = false;
  for (int i=0; i<N_INPUTS; i++) {
    if (localSource[i] == nullptr) { break; }
    overflowing = overflowing || localSource[i]->isOverflowing();
  }
  return overflowing;
}


void PipeInput::setOverflowSpeed(double speed) {
  for (int i=0; i<N_INPUTS; i++) {
    if (localSource[i] == nullptr) { break; }
    localSource[i]->setOverflowSpeed(speed);
  }
}




/****************************************************************\
 *                                                              *
 *                         PipeOutput                           *
 *                                                              *
\****************************************************************/


PipeOutput::PipeOutput(unsigned int pipeId, size_t length) 
: pipeId(pipeId), length(length) {}


void PipeOutput::setCanOutput() {
  useCan = true;
}


void PipeOutput::updateCanOverflow(Pipe *pipe, CanPipeOverflow overflow) {
  if (!useCan) { return; }
  if ((overflow.node == selfNodeId()) && (overflow.pipeId == pipeId)) {
    Serial.print(CanPipeOverflow::Action::SET_OVERFLOWING); Serial.print(" ");
    Serial.print(CanPipeOverflow::Action::SET_DRAINING); Serial.print(" ");
    Serial.println(overflow.action);
    if (overflow.action == CanPipeOverflow::Action::SET_OVERFLOWING) {
      Serial.println("overflowing");
      pipe->setOverflowing();
    } else if (overflow.action == CanPipeOverflow::Action::SET_DRAINING) {
      Serial.println("draining");
      pipe->setDraining();
    }
  }
}


void PipeOutput::sendDrained() {
  if (useCan) {
    sendCanBusPipeOverflow({ pipeId, selfNodeId(), CanPipeOverflow::Action::IS_DRAINED });
  }
}



unsigned int PipeOutput::count() {
  return m_count;
}


void PipeOutput::consumeFlow(PipeFlow &flow) {
  if (!flow.active) { return; }
  if (flow.offset + flow.length >= length) {
    m_count += flow.count;
  }
}


void PipeOutput::flush() {
  // Serial.print(pipeId); Serial.print("[out]: "); Serial.println(m_count);
  if ((m_count != prevCount) && useCan) {
    sendCanBusPipeOutput({pipeId, m_count});
  }
  prevCount = m_count;
}



/****************************************************************\
 *                                                              *
 *                           Pipe                               *
 *                                                              *
\****************************************************************/


// (constructor)
Pipe::Pipe(int pipeId, OctoWS2811 &strip, size_t start, size_t end) 
: length(stripLength(start, end)), pipeId(pipeId),
  output(pipeId, stripLength(start, end))
{
  renderers[0].configure(strip, start, end);
}

OverflowPipe::OverflowPipe(int pipeId, OctoWS2811 &strip, size_t start, size_t end)
: Pipe(pipeId, strip, start, end) {}

VirtualPipe::VirtualPipe(OctoWS2811 &strip, unsigned int length)
  : Pipe(-1, strip, 0, length) {}


void Pipe::attachInput(Pipe *pipe) {
  input.attachInput(pipe, this->pipeId);
}

void Pipe::attachCanInput(uint8_t node, unsigned int pipeId) {
  input.attachCanInput(node, pipeId);
}



void Pipe::addSegment(OctoWS2811 &strip, size_t start, size_t end) {
  for (int i=0; i<N_SEGMENTS; i++) {
    if (renderers[i].active == false) {
      renderers[i].configure(strip, start, end);
      return;
    }
  }
  Serial.print("!! WARNING !! Attempt to add too many segments on pipe "); Serial.println(pipeId);
}


// set whether the pipe should output CAN PipeOutput messages
void Pipe::setAsOutput(bool out) {
  output.setCanOutput();
}


void Pipe::setActivationLevel(unsigned int level) {
  activationLevel = level;
}


// create a self-flow
void Pipe::startFlow(unsigned int count, unsigned int level) {
  if (level >= activationLevel) {
    input.selfCount = count;
  } else {
    input.selfCount = 0;
  }
}

// end a self-flow
void Pipe::endFlow() {
  input.selfCount = 0;
}


unsigned int Pipe::getOutputCount() {
  return output.count();
}

void Pipe::setOverflowSpeed(double speed) {
  overflowSpeed = speed;
}
void Pipe::setDrainSpeed(double speed) {
  drainSpeed = speed;
}

bool Pipe::isOverflowing() {
  return overflowing;
}

void Pipe::setOverflowing() {
  overflowing = true;
  draining = false;
}

void Pipe::setDraining() {
  overflowing = false;
  if (overflowLevel < length) {
    draining = true;
  } else {
    input.setDraining();
  }
}

bool Pipe::drained() {
  return (!overflowing) && (overflowLevel == 0);
}


void Pipe::convertInputToMovingFlow() {
  if (inputFlow.active) {
    insertFlow(&inputFlow);
    inputFlow.active = false;
  }
}


// add a new moving flow
void Pipe::insertFlow(PipeFlow *f) {
  for (int i=0; i<N_FLOWS; i++) {
    if (!movingFlows[i].active) {
      movingFlows[i] = *f;
      return;
    }
  }
  Serial.print("!! WARNING !! no space left to insert flow on pipe ");
  Serial.println(pipeId);
}


void Pipe::updateCanInput(uint8_t srcId, CanPipeOutput output) {
  input.updateCanInput(srcId, output);
}

void Pipe::updateCanOverflow(uint8_t srcNode, CanPipeOverflow overflow) {
  input.updateCanOverflow(srcNode, overflow);
  output.updateCanOverflow(this, overflow);
}


// create/update the input flow
void Pipe::updateInput() {
  unsigned int count = input.countFlows(pipeId);
  if (count == 0) {
    // move any existing input flow to this->flows
    convertInputToMovingFlow();
    inputFlow.active = false;
  } else {
    // add a new flow if count has increased;
    if (input.countChanged()) {
      unsigned int gradOff = inputFlow.length + inputFlow.gradientOffset;
      convertInputToMovingFlow();
      inputFlow.active = true;
      inputFlow.offset = 0;
      inputFlow.length = speed;
      inputFlow.count = count;
      if (input.countIncreased()) {
        // new input flow, show gradient
        inputFlow.gradientOffset = 0;
      } else {
        inputFlow.gradientOffset = gradOff;
      }
    } else {
      inputFlow.advance(length, speed);
    }
  }
}


void Pipe::updateOverflow() {
  if (overflowLevel > 0 && input.drained() && !overflowing) {
    draining = true;
  }

  if (overflowing) {
    overflowLevel += overflowSpeed;
    if (overflowLevel > length) {
      overflowLevel = length;
      // overflowing = false;
      input.setOverflowing();
    }
  } else if (draining) {
    overflowLevel -= drainSpeed;
    if (overflowLevel < 0) {
      overflowLevel = 0;
      draining = false;
      output.sendDrained();
    }
  } 
}


void OverflowPipe::updateInput() {
  if (input.isOverflowing()) {
    Pipe::updateInput();
  } else {
    convertInputToMovingFlow();
  }
}


void OverflowPipe::updateOverflow() {
  if (overflowLevel > 0 && input.drained()) {
    draining = true;
  }

  if (overflowing) {
    overflowLevel += overflowSpeed;
    if (overflowLevel > length) {
      overflowLevel = length;
      // overflowing = false;
      input.setOverflowSpeed(DEFAULT_OVERFLOW_SPEED);
    }
  } else if (draining) {
    overflowLevel -= drainSpeed;
    if (overflowLevel < 0) {
      overflowLevel = 0;
      draining = false;
      output.sendDrained();
      input.setOverflowSpeed(0);
    }
  } 

}


// update all flows and outputs
void Pipe::update() {
  output.m_count = 0;
  // input flow
  updateInput();
  output.consumeFlow(inputFlow);

  // internal flows
  for (int i=0; i<N_FLOWS; i++) {
    movingFlows[i].advance(length, speed);
    output.consumeFlow(movingFlows[i]);
  }

  output.flush();
  updateOverflow();
}


void Pipe::render() {
  for (int i=0; i<N_SEGMENTS; i++) {
    PipeRenderer &renderer = renderers[i];
    if (renderer.active) {
      renderer.clear();
      for (int i=0; i<N_FLOWS; i++) {
        renderer.drawFlow(movingFlows[i]);
      }
  
      renderer.drawFlow(inputFlow);
      renderer.drawOverflow(overflowLevel);
      // Serial.print(pipeId); Serial.print(": "); Serial.println(overflowLevel);
    }
  }
}


void VirtualPipe::setId(unsigned int id) {
  pipeId = id;
}


void VirtualPipe::render() {}


/* ================================================================ *\
 *                                                                  *
 *                            RENDERING                             *
 *                                                                  *
 * ================================================================ */


void PipeRenderer::configure(OctoWS2811 &strip, size_t start, size_t end) {
  this->start = start;
  this->end = end;
  this->strip = &strip;
  this->active = true;
}


// lerp for uint8_t values
uint8_t ulerp(uint8_t a, uint8_t b, float theta) {
  double aa = a;
  double bb = b;
  double da = (1-theta) * aa;
  double db = (theta) * bb;
  return da + db;
}


// lerp for color_t values
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


color_t bgColor(int stripIndex) {
  double x = ((double)stripIndex) / 30.0;
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


void PipeRenderer::drawPixel(int index, color_t c) {
  strip->setPixel(index, c.r, c.g, c.b);
}


void PipeRenderer::clear() {
  for (int i=0; i<length(); i++) {
    drawPixel(stripIndex(i), { 0, 0, 0 });
  }
}


void PipeRenderer::drawFlow(PipeFlow &flow) {
  if (!flow.active) { return; }
  for (unsigned int i=0; i<flow.length; i++) {
    unsigned int idx = i + flow.offset;
    if (idx < length()) {
      unsigned int stripIdx = stripIndex(idx);
      color_t leader = leaderColor(stripIdx);
      color_t tail = bgColor(stripIdx);

      double x = flow.length + flow.gradientOffset - i;
      x -= 20;
      double alpha = x < 0 ? 1.0 : exp(-x/20);

      drawPixel(stripIdx, alphaBlend(tail, leader, alpha));
    }
  }
}


void PipeRenderer::drawOverflow(double level) {
  for (int i=0; i<level; i++) {
    drawPixel(stripIndex(length() - i - 1), leaderColor(0));
  }
}


unsigned int PipeRenderer::stripIndex(unsigned int i) {
  if (start < end) {
    return start+i;
  } else {
    return start-i;
  }
}


unsigned int PipeRenderer::length() {
  if (start < end) {
    return end - start;
  } else {
    return start - end;
  }
}



