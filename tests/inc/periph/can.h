#ifndef __CAN_H
#define __CAN_H
#include <stdint.h>
#include <stdbool.h>
#include "uart.h"

typedef enum {
  CAN_1,
  CAN_2,
  CAN_3
} CAN_bus;


// TODO this should be taken out once CANlib is integrated
typedef struct {
  uint32_t id;
  uint8_t len;
  uint8_t data[8];
  bool extended;
} Frame;

Frame read_can(CAN_bus bus);
void send_can(CAN_bus bus, Frame *frame);
int can_init(CAN_bus bus);

#endif // __CAN_H
