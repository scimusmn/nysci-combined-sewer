#pragma once
#include <FlexCAN_T4.h>


typedef enum : uint8_t {
  INPUT_LEVELS,
  PIPE_OUTPUT,
  PIPE_OVERFLOW,
  COLOR_SWAP,
} MessageType;


// struct to store input level information
typedef struct {
  uint8_t rainFlow;
  uint8_t toiletFlow;
  uint8_t washerFlow;
  uint8_t dishWasherFlow;
  uint8_t showerFlow;
} InputLevels;


typedef struct {
  unsigned int pipeId;
  unsigned int count;
} CanPipeOutput;



typedef struct {
  unsigned int pipeId;
  uint8_t node;
  enum Action : uint8_t {
    SET_OVERFLOWING,
    SET_DRAINING,
    IS_DRAINED,
  } action;
} CanPipeOverflow;


// callbacks for receiving data
// MUST BE USER DEFINED
// src: the id of the source node
void processInputLevels(uint8_t src, InputLevels levels);
void processPipeOutput(uint8_t src, CanPipeOutput output);
void processPipeOverflow(uint8_t src, CanPipeOverflow o);
// transmit input levels via CAN bus
void sendCanBusInputLevels(InputLevels levels);
void sendCanBusPipeOutput(CanPipeOutput output);
void sendCanBusPipeOverflow(CanPipeOverflow o);
void sendCanSwapColors(bool swap);


// function to set up the CAN bus
// MUST BE CALLED BEFORE DOING ANY OTHER CAN STUFF
void setupCan(uint8_t id);


uint8_t selfNodeId();
bool switchOverflowColor();

// send generic data
void sendMessage(MessageType type, void *data);
