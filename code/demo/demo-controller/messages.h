#pragma once
#include <FlexCAN_T4.h>


#define INPUT_LEVELS_MSG 0

// struct to store input level information
typedef struct {
  uint8_t rainFlow;
  uint8_t toiletFlow;
  uint8_t washerFlow;
  uint8_t dishWasherFlow;
  uint8_t showerFlow;
} InputLevels;
// callback for receiving input level data
// MUST BE USER DEFINED
// src: the id of the source node
// levels: the new levels
void processInputLevels(uint8_t src, InputLevels levels);
// transmit input levels via CAN bus
void sendInputLevels(uint8_t src, InputLevels levels);


// function to set up the CAN bus
// MUST BE CALLED BEFORE DOING ANY OTHER CAN STUFF
void setupCan();

// send generic data
void sendMessage(uint8_t type, uint8_t nodeId, void *data);
// process generic CAN message (used internally)
void processMessage(const CAN_message_t &msg);
