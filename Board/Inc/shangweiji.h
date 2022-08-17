#ifndef __shangweiji_H
#define __shangweiji_H

#include "common.h"
#include "stdlib.h"
#include "include.h"




void uart_IN_init(void);
void uart0_handler(void);
extern struct NUM_GET NUM_A;
extern struct NUM_GET NUM_B;
extern struct NUM_GET NUM_C;
extern struct NUM_GET NUM_D;
extern struct NUM_GET NUM_E;
extern void NUM_GET(void);
extern void NUM_Send(void);

#endif


