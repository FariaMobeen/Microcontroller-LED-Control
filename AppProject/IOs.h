// IOs.h 

#ifndef IOs_H
#define IOs_H

// Function to initialize IO ports
void IOinit();

// Configure timer decrement, pause, reset and LED output
void UpdateTimer();

// Display timer on terminal
void UpdateDisplay();

void StartInterrupt(); //Enable CN interrupt

// Configuring the cases for each pushbutton
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt();

#endif

