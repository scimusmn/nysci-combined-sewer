#pragma once
#include <FlexCAN_T4.h>


typedef enum : uint8_t {
  PIPE_OUTPUT,
  PIPE_OVERFLOW,
  INPUT_LEVELS,
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
} PipeOutput;

// callbacks for receiving data
// MUST BE USER DEFINED
// src: the id of the source node
void processInputLevels(uint8_t src, InputLevels levels);
void processPipeOutput(uint8_t src, PipeOutput output);
// transmit input levels via CAN bus
void sendCanBusInputLevels(InputLevels levels);
void sendCanBusPipeOutput(PipeOutput output);


// function to set up the CAN bus
// MUST BE CALLED BEFORE DOING ANY OTHER CAN STUFF
void setupCan(uint8_t id);

// send generic data
void sendMessage(MessageType type, void *data);
