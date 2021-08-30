#include <xc.h>
#include <plib.h>
#include <stdint.h>
#include <xc.h>

#include "INPUT_CAPTURE.h"
#include "ADC.h"
//#include "UART.h"
#include "system.h"


    volatile float perioD = 0;
    volatile float AdcValue = 0;
    volatile float old_ticks = 0;

void main(void) {

 //   unsigned int new_ticks = 0;
 //   unsigned int old_ticks = 0;
    //unsigned int


    Timer32Initiate();
    InputCapInitiate();
    AdcInitiate();
    UART_Initiate();

    INTEnableSystemMultiVectoredInt();

/*
    TRISAbits.TRISA0 = 0;
    ANSELAbits.ANSA0 = 0;
    LATAbits.LATA0 = 0;
*/

    while(1) {
 //       Nop();
            }

}