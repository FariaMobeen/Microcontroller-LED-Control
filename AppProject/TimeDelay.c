#include "TimeDelay.h"
#include "ChangeClk.h"

void delay_ms(uint16_t time_ms)
{
    NewClk(32); // Set the clock speed to 32 kHz
    T2CONbits.TSIDL = 0; //operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCKPS = 0b00; //Prescaler=1
    T2CONbits.TCS = 0; // use internal clock
    TMR2 = 0; // Clear TMR2 register
    PR2 = time_ms*16; // produces delay of 16*2/fclk where fclk = 32
    T2CONbits.TON = 1; //Starts Timer 2
    IPC1bits.T2IP = 7; //7 is highest priority, 1 is lowest
    IFS0bits.T2IF = 0; //Clear Timer 2 flag
    IEC0bits.T2IE = 1; // Enable Timer 2 interrupt
    Idle(); //Stays here in low power till interrupt 
    return;
}


// Timer 2 interrupt service routine ISR built into the Microcontroller
// as in slide11. Keep code in ISRs short
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF=0; //Clear timer 2 interrupt flag
    T2CONbits.TON=0; //stop timer
    TMR2 = 0;
    //TMR2flag = 1; // global variable created by user if required
    // figure out if a timer interrupt occurred somewhere in code
    return;
}

