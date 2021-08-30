#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <stdint.h>
#include <math.h>
#include "INPUT_CAPTURE.h"
#include "UART.h"

volatile unsigned int temp;


void InputCapInitiate(void)
{
/*Configure the Input Capture*/

    //First, let's take care of PPS
    TRISAbits.TRISA1 = 1; // set pin 3 as input
    ANSELAbits.ANSA1 = 0; // set pin 3 as digital
    CNPDAbits.CNPDA1 = 1;
    IC3R = 0x0000; // Map the Input Capture PPS to pin 3
                   // see Manual 12 IO datasheet page 10

    //Second, configure the Input Capture.
    IC3CONbits.ON = 0; /*Input Capture Module Enable bit
1 = Module enabled
0 = Disable and*/
    IC3CONbits.C32 = 1; /*32-bit Capture Select bit
1 = 32-bit timer resource capture
0 = 16-bit timer resource capture*/
/*    IC3CONbits.ICTMR = 1;  Timer Select bit (Does not affect timer selection when C32 (ICxCON<8>) is ?1?)
//0 = Timer3 is the counter source for capture
//1 = Timer2 is the counter source for capture */
    IC3CONbits.ICI = 0;/*Interrupt Control bits
11 = Interrupt on every fourth capture event
10 = Interrupt on every third capture event
01 = Interrupt on every second capture event
00 = Interrupt on every capture event*/
    IC3CONbits.ICM = 5; /*Input Capture Mode Select bits
111 = Interrupt-Only mode (only supported while in Sleep mode or Idle mode)
110 = Simple Capture Event mode ? every edge, specified edge first and every edge thereafter
101 = Prescaled Capture Event mode ? every sixteenth rising edge
100 = Prescaled Capture Event mode ? every fourth rising edge
011 = Simple Capture Event mode ? every rising edge
010 = Simple Capture Event mode ? every falling edge
001 = Edge Detect mode ? every edge (rising and falling)
000 = Capture Disable mode */

    //Configure the Input Capture interrupts
    IFS0bits.IC3IF = 0; // Clear the "Input Capture" interrupt flag
    IPC3bits.IC3IP = 5; // Set the "Input Capture" priority
    IEC0bits.IC3IE = 1; // Enable the "Input Capture" interrupt

    //Switch the Input Capture on since its configured now.
    IC3CONbits.ON = 1;

    //Clear the 32-bit timer so the time spent on the code above is dismissed
    TMR2 = 0;
    TMR3 = 0;
//--------------------------------------------------------------------



}

// Configure the input capture interrupt handler
void __ISR(_INPUT_CAPTURE_3_VECTOR, ipl5) InputCaptureHandler(void)
{
    //float ticks;
    extern float perioD;
    extern float old_ticks;
    float new_ticks = 0;

    while(!IC3CONbits.ICBNE);

   // Store the timer value of the capture event into CaptureTime variable
    new_ticks = IC3BUF; // ticks = mIC3ReadCapture();
        //ticks = IC3BUF;

    perioD = new_ticks - old_ticks;
    old_ticks = new_ticks;
    IFS0bits.IC3IF = 0;
//    Nop();
}







