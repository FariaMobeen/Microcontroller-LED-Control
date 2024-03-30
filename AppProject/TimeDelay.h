#ifndef TIMEDELAY_H
#define TIMEDELAY_H

#include "xc.h"
#include <stdint.h>

// Function to delay in milliseconds
void delay_ms(uint16_t time_ms);

// Timer 2 interrupt service routine
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt();

#endif


