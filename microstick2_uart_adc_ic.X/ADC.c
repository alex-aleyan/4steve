#include <plib.h>
#include "ADC.h"

int AdcInitiate(void)
{
        CNPDAbits.CNPDA0 = 1;
	AD1CON1bits.ADON = 0;
        // let's turn off the ADC so we don't configure the ADC settings with
        // while the ADC is on. Use PIC32 Manual 17 page 6

        TRISAbits.TRISA0 = 1;
        // Let's configure the desire pin as an input
        // so we can read off of it (TRIS port A pin 0)
        // Use PIC32 Manual 12 page 3

        ANSELAbits.ANSA0 = 1;
        // also, the whole port A can be set using ANSELA = 1;
        // set pins of port A as analog.
        // Use PIC32 Manual 12 page 4

        ANSELB =0;
        // set pins of port B as digital.
        // Use PIC32 Manual 12 page 4

        AD1CON1 = 0x00E0;   // auto convert after end of sampling
        // AD1CON1 = 0000 0000 1110 0000
        // Use PIC32 Manual 17 page 6

        // ASAM = 0; ADC sample auto-start bit
        //           1 = Sabmpling begins immediately after last
        //           conversion completes. SAMP bit is automatically set.
        //           0 = Sampling begins when SAMP bit is set.

        // SAMP = 0; ADC Sample Enable bit
        //           1 = The ADC SHA is sampling
        //           0 = The ADC sample/hold amplifier is holding
        //           When ASAM = 0, writing ?1? to this bit starts sampling.
        //           When SSRC = 000, writing ?0? to this bit will end sampling and start conversion.

        // DONE = 0; Analog-to-Digital Conversion Status bit(2)
        //           1 = Analog-to-digital conversion is done
        //           0 = Analog-to-digital conversion is not done or has not started
        //           Clearing this bit will not affect any operation in progress.

        // CLRASAM = 0; Stop Conversion Sequence bit (when the first ADC interrupt is generated)
        //           1 = Stop conversions when the first ADC interrupt is generated. Hardware clears the ASAM bit when the ADC interrupt is generated.
        //           0 = Normal operation, buffer contents will be overwritten by the next conversion sequence

        // SSRC = 111 Conversion Trigger Source Select bits
        //           111 = internal counter end sampling and starts conversion (auto convert);
        //           010 Timer 3 period match ends sampling and starts conversion

        AD1CON2 = 0; // use MUXA, AVss and AVdd are used as Vref+/-
        // Use PIC32 Manual 17 page 8

        // ALTS = 0; Alternate Input Sample Mode Select bit
        //           1 = Uses MUX A input multiplexer settings for first sample, then alternates between MUX B and
        //           MUX A input multiplexer settings for all subsequent samples
        //           0 = Always use MUX A input multiplexer settings

        // BUFM = 0; ADC Result Buffer Mode Select bit
        //           1 = Buffer configured as two 8-word buffers, ADC1BUF(7...0), ADC1BUF(15...8)
        //           0 = Buffer configured as one 16-word buffer ADC1BUF(15...0.)

        // SMPI = 0; Sample/Convert Sequences Per Interrupt Selection bits
        //           1111 = Interrupts at the completion of conversion for each 16th sample/convert sequence
        //           1110 = Interrupts at the completion of conversion for each 15th sample/convert sequence
        //           ?
        //           ?
        //           ?
        //           0001 = Interrupts at the completion of conversion for each 2nd sample/convert sequence

        // BUFS = 0; Buffer Fill Status bit
        //           Only valid when BUFM = 1 (ADRES split into 2 x 8-word buffers).
        //           1 = ADC is currently filling buffer 0x8-0xF, user should access data in 0x0-0x7
        //           0 = ADC is currently filling buffer 0x0-0x7, user should access data in 0x8-0xF

        // CSCNA = 0; Scan Input Selections for CH0+ SHA Input for MUX A Input Multiplexer Setting bit
        //           1 = Scan inputs
        //           0 = Do not scan inputs

        // OFFCAL = 0; Input Offset Calibration Mode Select bit
        //           1 = Enable Offset Calibration mode? ú?VINH and VINL of the SHA are connected to VR-
        //           0 = Disable Offset Calibration mode);§ÜThe inputs to the SHA are controlled by AD1CHS or AD1CSSL

        // VCFG = 000; Voltage  refference              configuration bits
        //           BITS       ADC Vr+                 ADC Vr-
        //
        //           000        AVdd                    AVss
        //           001        External Vref+ pin      AVss
        //           010        AVdd                    External Vref- pin
        //           1xx        AVdd                    AVss

        AD1CON3 = 0x1F02;   // system clock,max sample time = 31Tad, Tad = 2 x Tcy = 125ns >75ns
        // AD1CON3 = 0001 1111 0000 0010
        // Use PIC32 Manual 17 page 9

        // ADCS = 0000 0010 ;ADC Conversion Clock Select bits(1)
        //           11111111 = TPB ? 2 ? (ADCS<7:0> + 1) = 512 ? TPB = TAD
        //           ?
        //           ?
        //           ?
        //           00000001 = TPB ? 2 ? (ADCS<7:0> + 1) = 4 ? TPB = TAD
        //           00000000 = TPB ? 2 ? (ADCS<7:0> + 1) = 2 ? TPB = TAD

        // SAMC = 1 1111; Auto-sample Time bits
        //           11111 = 31 TAD
        //           ?
        //           ?
        //           ?
        //           00001 = 1 TAD
        //           00000 = 0 TAD (Not allowed)

        // ADRC = 0; ADC Conversion Clock Source bit
        //           1 = ADC internal RC clock
        //           0 = Clock derived from Peripheral Bus Clock (PBCLK)





        AD1CHS =0x00000;
        // Next, the AD1CHS registrer's bits are configured so that:
        // Use PIC32 Manual 17 page 10

        // CH0SA = 0000; Positive Input Select bits for MUX A Multiplexer Setting
        //          1111 = Channel 0 positive input is AN15
        //          1110 = Channel 0 positive input is AN14
        //          1101 = Channel 0 positive input is AN13
        //          ?
        //          ?
        //          ?
        //          0000 = Channel 0 positive input is AN0

        // CH0NA = 0; Negative Input Select bit for MUX A Multiplexer Setting
        //          1 = Channel 0 negative input is AN1
        //          0 = Channel 0 negative input is VR-

        // CH0SB = 0000; Positive Input Select bits for MUX B
        //          1111 =Channel 0 positive input is AN15
        //          1110 =Channel 0 positive input is AN14
        //          1101 =Channel 0 positive input is AN13
        //          ?
        //          ?
        //          ?
        //          0001 = Channel 0 positive input is AN1
        //          0000 = Channel 0 positive input is AN0

        // CH0NB = 0; Negative Input Select bit for MUX B
        //          1 = Channel 0 negative input is AN1
        //          0 = Channel 0 negative input is VR-

        AD1CSSL = 0; // no scanning since we manually setting the ADC.
        // Use PIC32 Manual 17 page 11

        // ADC Input Pin Scan Selection bits
        //          1 = Select ANx for input scan
        //          0 = Skip ANx for input scan
        IFS0bits.AD1IF = 0;
        IPC5bits.AD1IP = 4;
        IEC0bits.AD1IE = 1;//1;

        AD1CON1bits.ADON = 1; // turn on the ADC, but the sampling isn't launched yet
        AD1CON1bits.SAMP = 1;
        return 1;
}


void __ISR(_ADC_VECTOR, ipl4 ) ADC_Interrupt(void)

{ 
    extern float AdcValue;
    int adcvalue0 = 0;      // Declare variable to be used for return.
    int *adc16ptr0;         // Declare pointer to retreave the ADC result
                            // contained at &ADC1BUF0
                            

    while (!AD1CON1bits.DONE);   // wait to complete the conversion

    adc16ptr0 = &ADC1BUF0 ; // Use the pointer to point to ADC result word ADC1BUF0.
                            // see table 17-1 of ADC manual.
    AdcValue = *adc16ptr0; // Now, we assign the value (the pointer point to) to AdcValue variable.
                            // It is unneccessary step but we do it the the function look cleaner.
    //Nop();
    AD1CON1bits.SAMP = 1;       // start sampling, automatic conversion will follow
    IFS0bits.AD1IF = 0;
}