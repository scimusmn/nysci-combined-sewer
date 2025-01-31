#include "messages.h"
#include <FlexCAN_T4.h>
static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> MsgCan;


static uint8_t selfId = 0xff;
bool overflowColorSwap = false;


void processMessage(const CAN_message_t &msg) {
  // Serial.println("rx");
  uint8_t type = msg.id >> 4;
  // Serial.println(type);
  uint8_t src = msg.id & 0xf;
  switch(type) {
  case PIPE_OUTPUT:
    do {
      CanPipeOutput output;
      memcpy(&output, msg.buf, sizeof(CanPipeOutput));
      processPipeOutput(src, output);
    } while(0);
    break;
  case PIPE_OVERFLOW:
    do {
      CanPipeOverflow overflow;
      memcpy(&overflow, msg.buf, sizeof(CanPipeOverflow));
      Serial.println(overflow.action);
      Serial.print("sizes: "); Serial.print(sizeof(CanPipeOverflow)); Serial.print(" ");
      Serial.println(8*sizeof(uint8_t));
      processPipeOverflow(src, overflow);
    } while(0);
    break;
  case INPUT_LEVELS:
    do {
      InputLevels levels;
      memcpy(&levels, msg.buf, sizeof(InputLevels));
      processInputLevels(src, levels);
    } while(0);
    break;
  case COLOR_SWAP:
    memcpy(&overflowColorSwap, msg.buf, sizeof(bool));
    break;
  default:
    // do nothing
    Serial.print("WARNING: unknown message type: "); Serial.println(type);
    break;
  }
}



void setupCan(uint8_t id) {
  selfId = id;
  MsgCan.begin();
  MsgCan.setBaudRate(1000000);
  MsgCan.setMaxMB(16);
  MsgCan.enableFIFO();
  MsgCan.enableFIFOInterrupt();
  MsgCan.onReceive(processMessage);

  // check to ensure our messages fit in one frame
  if (sizeof(InputLevels) > sizeof(CAN_message_t::buf)) {
    Serial.println("InputLevels are too large!!");
  }
  if (sizeof(CanPipeOutput) > sizeof(CAN_message_t::buf)) {
    Serial.println("CanPipeOutput are too large!!");
  }
  if (sizeof(CanPipeOverflow) > sizeof(CAN_message_t::buf)) {
    Serial.println("CanPipeOverflow are too large!!");
  }
}


void sendMessage(MessageType type, void *data, size_t sz) {
  CAN_message_t msg;
  msg.id = (type << 4) | selfId;
  memcpy(msg.buf, data, sz);
  msg.len = sz;
  MsgCan.write(msg);
}




void sendCanBusInputLevels(uint8_t src, InputLevels levels) {
  sendMessage(INPUT_LEVELS, &levels, sizeof(InputLevels));
}


void sendCanBusPipeOutput(CanPipeOutput output) {
  sendMessage(PIPE_OUTPUT, &output, sizeof(CanPipeOutput));
}


void sendCanBusPipeOverflow(CanPipeOverflow o) {
  Serial.print("tx overflow "); 
  Serial.print(o.node); Serial.print(":"); Serial.print(o.pipeId);
  Serial.print(" "); Serial.println(o.action);
  sendMessage(PIPE_OVERFLOW, &o, sizeof(CanPipeOverflow));
}


void sendCanSwapColors(bool swap) {
  sendMessage(COLOR_SWAP, &swap, sizeof(bool));
}


uint8_t selfNodeId() {
  return selfId;
}

bool switchOverflowColor(){
  return overflowColorSwap;
}
