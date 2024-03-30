#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "UART2.h"
unsigned int minutes = 0;
unsigned int seconds = 0;
int alarm = 0;
int pb3count = 0;


void IOinit() {
    // configuring TRISx, CNPUx, and CNxIE interrupt bits
    
    // Configure CN Peripheral on RA2
    AD1PCFG = 0xFFFF; // disable ADC on RA2
    TRISAbits.TRISA2 = 1; // Set RA2 as input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA2/CN30, configures RA2 as the pushbutton PB1 input
    CNEN2bits.CN30IE = 1; // Enable CN on RA2
    
    // Configure CN Peripheral on RA4
    TRISAbits.TRISA4 = 1;  //Set RA4 as input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0, configures RA4 as the pushbutton PB2 input
    CNEN1bits.CN0IE = 1; // Enable CN on RA4
    
    // Configure CN Peripheral on RB4
    TRISBbits.TRISB4 = 1;  //Set RB4 as input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RB4/CN1, configures RB4 as the pushbutton PB3 input
    CNEN1bits.CN1IE = 1; // Enable CN on RB4
    
    // Configure CN Interrupts
    IPC4bits.CNIP = 6; // set priority
    IFS1bits.CNIF = 0; //clear flag
    IEC1bits.CNIE = 1; //enable interrupt
    
    return;

}

void StartInterrupt(){
    IEC1bits.CNIE = 1; // Enable CN interrupt
    return;
}
void UpdateDisplay() {
    Disp2String("\r");
    Disp2Dec(minutes);
    Disp2String("m : ");
    Disp2Dec(seconds);
    Disp2String("s");

    if (alarm) { // Display ALARM message
        Disp2String("--ALARM");
    } else {
        // Overwrite the alarm message position
        Disp2String("       ");
    }
    return;
}

void UpdateTimer() {
    //Disable CN interrupt
    IEC1bits.CNIE = 0;
    while (1) {
        if (seconds > 0) { //Decrement seconds
            seconds--;
        } else {
            if (minutes > 0) { //Decrement minutes
                minutes--;
                seconds = 59;
            }
        }
        if (minutes == 0 && seconds == 0) {
            break;
        }
        UpdateDisplay();
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Toggle LED on RB8 to high
            
        // for pausing/starting and resetting the timer
        while (PORTBbits.RB4 == 0) {
            LATBbits.LATB8 = 0; //Toggle LED on RB8 to low
            UpdateDisplay();
            IEC1bits.CNIE = 1; //Enable CN interrupt
            return;   
        }
    }
    //On short press, update display when timer reaches zero
    alarm = 1;
    LATBbits.LATB8 = 1;
    UpdateDisplay();
    
    //Enable CN interrupt
    IEC1bits.CNIE = 1;
    return;
}
    

// Configure CN Interrupt on RA2, RA4, and RB4
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    while (PORTAbits.RA2 != 0 && PORTAbits.RA4 != 0 && PORTBbits.RB4 != 0 )
    { 
        Idle(); //Stays here in low power till interrupt 
    }
    //while RA2 push button is pressed 
    while (PORTAbits.RA2 == 0){
        minutes = minutes + 1; //Increment minutes
        alarm = 0;
        UpdateDisplay();
        LATBbits.LATB8 = 0; 
        if (minutes == 59){
            minutes = 0;
        }
    }
    //while RA4 push button is pressed 
    while (PORTAbits.RA4 == 0){
        seconds = seconds + 1; //Increment seconds
        alarm = 0;
        UpdateDisplay();
        LATBbits.LATB8 = 0; 
        if (seconds == 59){
            seconds = 0;
        }
    }
    //while RB4 push button is pressed 
    while (PORTBbits.RB4 == 0) {
        pb3count = pb3count + 1;
        
        if (pb3count == 1) { // Detecting a short press
            UpdateTimer();
        } 
        else if (pb3count >= 3) { // Detecting a long press
            minutes = 0;
            seconds = 0;
            alarm = 0;
            UpdateDisplay();
            LATBbits.LATB8 = 0;
        }
    }
    //Reset the pb3count 
    pb3count = 0;

}

