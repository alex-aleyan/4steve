
#ifndef INPUT_CAPTURE_H
#define	INPUT_CAPTURE_H

void InputCapInitiate(void);
//extern float perioD;


#endif	/* INPUT_CAPTURE_H */






/*********************************************************************
 *
 *      PIC32MX Input Capture Example
 *
 *********************************************************************
 * FileName:        incap_capture_event.c
 * Dependencies:    plib.h
 *
 * Processor:       PIC32MX
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *********************************************************************
 * A Rafiq - 3/27/07  Initial Version
 *         - 12/12/12 Updated for new part support
 *********************************************************************
 * The purpose of this example code is to demonstrate the PIC32MX
 * peripheral library macros and functions supporting the Input
 * Capture module and its various features.
 *
 * Platform: Explorer-16 with PIC32MX PIM
 *
 * Description:
 *	When a rising edge is output to the input capture 1 pin,
 *      it triggers an interrupt which then stores the timer value
 *      of the incap event into the 'CaptureTime' variable. When
 *      run in debug mode, the value of the 'CaptureTime' variable
 *      can be seen in the watch window.
 *
 * Notes:
 *    - In this example, pin RD8 is used for input capture on all
 *      devices except for the PIC32MX 2xx series PIMS. On the 2xx
 *      series PIMS, pin RA2 is used for input capture.
 *    - The timer is running in 32bit mode at a frequency of ~39KHz,
 *      with a period of 0xFFFFFFFF (~4 billion). The input capture
 *      module can therefore capture any event within 28 hours
 *      (before the timer rolls over) with 26us of accuracy.
 ********************************************************************/
/*#include <plib.h>

#if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__) || (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 10 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#define SYS_FREQ (80000000L)

#elif defined (__32MX220F032D__) || (__32MX250F128D__)
// Configuration Bit settings
// SYSCLK = 40 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 10 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_2, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_4
#define SYS_FREQ (40000000L)
#endif

#define PERIOD  0xFFFFFFFF // Timer period of ~ 4 billion (max period)

unsigned int CaptureTime; // Used to store the captured timer value

int main(void)
{
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above.
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    #if defined (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
    PPSInput(4,IC1, RPD8); // Use pin RD8 for input capture
    #elif defined (__32MX220F032D__) || defined (__32MX250F128D__)
    PPSInput(3,IC1, RPA2); // Use pin RA2 for input capture
    #endif

    // Setup Timer 23 (timer 2 + timer 3, 32bit time)
    // Use PBCLK as source, FPB/256, Timer running at (10E6)/256 = 39KHz
    OpenTimer23(T23_ON | T23_SOURCE_INT | T23_PS_1_256 | T23_32BIT_MODE_ON, PERIOD);

    // Enable Input Capture Module 1
    // - Capture every rising edge
    // - Interrupt on first capture
    // - Use Timer 23 source
    // - Capture rising edge first
    OpenCapture1(IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_CAP_32BIT | IC_FEDGE_RISE | IC_ON);

    // Set up the input capture 1 interrupt with a priority of 2, subpriority of 0
    INTEnable(INT_IC1, INT_ENABLED);
    INTSetVectorPriority(INT_INPUT_CAPTURE_1_VECTOR, INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_INPUT_CAPTURE_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

    // Enable multi-vector interrupts
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

    while(1)
    {
        // Waiting for input capture event (stuck in this loop)
    }

    // Not executed
    CloseCapture1();
    CloseTimer23();

    return 0;
}

// Configure the input capture interrupt handler
void __ISR(_INPUT_CAPTURE_1_VECTOR, ipl2) InputCapture_Handler(void)
{
    // Clear the interrupt flag
    INTClearFlag(INT_IC1);

    // Wait for capture data to be ready
    while(!mIC1CaptureReady());

   // Store the timer value of the capture event into CaptureTime variable
   CaptureTime = mIC1ReadCapture();
   // Process data
   // ...
}





  */