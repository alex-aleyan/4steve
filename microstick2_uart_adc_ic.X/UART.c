#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <stdint.h>
#include <math.h>

#include "UART.h"

void UART_Initiate(void)
{
    /* Configure PPS for UART pins
     From the MicrostickII Info sheet we find that the header
     on the bottom of the microstick has the following pinout:
     1 - GND
     2 -
     3 -
     4 - PIN21 / RXD
     5 - PIN22 / TXD
     6 -
     From the device specific datasheet (pg) we see:
     pin 21: PGED2 / RPB10 / D+ / CTED11 / RB10
     pin 22: PGEC2 / RPB11 / D- / RB10

     Looking at table 11-1 on pg. 144 of the device datasheet we see that
     U2RX can be mapped to RPB11
     From that same table we notice U1RX can't be mapped to either RPB10 or
     RPB11 and U2RX can't be mapped to RPB10 thus U2RX must be mapped to RPB11
    */
    // map U2RX to RPB11 (see device datasheet table 11-1 on pg 144)
    U2RXRbits.U2RXR = 3;

    // map pin RPB10 to U2TX (see device datasheet 11-2 on pg 146)
    RPB10Rbits.RPB10R = 2;

    // Initalize all settings to default by setting UxMODE and UxSTA to 0
    U2MODE = 0 ;
    U2STA = 0 ;

    // Set the baud rate to 9600bps (see Equation 21-2 of the UART manual)
    // Note: if we keep the peripheral clock at 1MHz we need to use the high
    // speed baud rate mode or else our baud rate error will be too high.

    U2BRG = (1000000/(4*BAUD_RATE))-1;

    // Clear the UART receive interrupt flag (see device datasheet table 4-2 on
    // on page 41)
    IFS1bits.U2RXIF = 0;

    // Set the UART receive interrupt priority to 4 (see device datasheet table
    // 4-2 on page 42)
    IPC9bits.U2IP = 1;

    // Turn on high speed BRG (see device datasheet register 18-1 on pg 179)
    U2MODEbits.BRGH = 1;

    // Turn the UART module on (see device datasheet register 18-1 on pg 178)
    U2MODEbits.ON = 1;

    // Enable the UART TX module (see device datasheet register 18-2 on pg 180)
    U2STAbits.UTXEN = 1;

    // Enable the UART RX module (see device datasheet register 18-2 on pg 180)
    U2STAbits.URXEN = 1;

    // Enable UART RX interrupts (see device datasheet table 4-2 on
    // on page 41)
IEC1bits.U2RXIE = 1;
}

// UART 2 interrupt fires for RX and TX interrupts so we must check specifically
// for the RX interrupt flag
void __ISR(_UART_2_VECTOR, ipl1 ) UART_Interrupt(void)
{
 IFS1bits.U2TXIF = 0;
    extern float perioD;
    extern float AdcValue;
    //float periodiC = 0;
    //periodiC = period();
    uint8_t request;
    uint8_t count;


//sss = (float)(period/3906);


        if( IFS1bits.U2RXIF)
    {
        // while data is available to be read (UxSTAbits.URXDA)
        while(U2STAbits.URXDA)
        {
            // read one byte from the UART RX register and save it to c
        request = U2RXREG;
        //U2TXREG = perioD;
            switch(request)
            {
                // Displays "Help" screen
                case 't':
                    printf(" %f ", perioD);
                    U2STAbits.UTXBRK = 1;
                    break;
                case 'T':
                    printf(" %f ", AdcValue);
                    U2STAbits.UTXBRK = 1;
                    break;
            }
   IFS1bits.U2TXIF = 0;

        }

    }
 IFS1bits.U2TXIF = 0;
}

