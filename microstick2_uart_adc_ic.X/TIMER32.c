#include <stdio.h>
#include <stdlib.h>
#include "TIMER32.h"
#include <plib.h>


void Timer32Initiate(void)
{
    // Configure the 32-bit Timer 32 and its Interrupts (so it can be used by Input Capture Module)

    //First Configure the timer itself
    T2CONbits.ON = 0; // Turn off the timer 2 before configuring.
    T2CONbits.TCS = 0; // Set the peripheral clock as the source clock.
    T2CONbits.T32 = 1; // Set the timer to the 32-bit format.
    T2CONbits.TCKPS = 0; /* Set the clock division to 256.
11 = 1:256 prescale value
10 = 1:64 prescale value
01 = 1:8 prescale value
00 = 1:1 prescale value*/
    T2CONbits.TGATE = 0; // Disable the Gated Accumulation.
    PR2 = 0xFFFF; // set the Timer2 period.
    PR3 = 0xFFFF; // Set the Timer3 period.
    /*Remember: 32bit register PR32 = 16 bit register PR3 and 16 bit registerPR2.*/
    /* T2CONbits.TGATE = 0; as I understand, in this mode, the TMR register
     * counts up and is reset based on the external signal (as long as it's high,
     * it will count). As a result, we can measure the time period between the
     * resets of the external signal  */

    // Second, configure the timer32 interrupts
    IEC0bits.T3IE = 0; //Disable the Interrupt before configuring it.
    IFS0bits.T3IF = 0; //let's clear the interrupt flag.
    IPC3bits.T3IP = 3; //let's set the Interrupt Priority (should match the ISR)
    IEC0bits.T3IE = 1; //let's enable the interrupt.

    // Enave the timer 2
    T2CONbits.ON = 1; // Enable the Timer2, you don't need to enable the Timer3.

}


void __ISR(_TIMER_23_VECTOR, ipl3) Timer23Handler(void)
{
    LATAbits.LATA0 = ~LATAbits.LATA0;
    IFS0bits.T3IF = 0;/*Remember, for 32 bit timer only Timer3 Interrupt
                        Flag is use*/
}