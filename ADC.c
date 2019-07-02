#include <xc.h> //Header file required for all MPLAB XC8 source files
#define _XTAL_FREQ 3276800 //Required for the __delay_us and __delay_ms macros

void ADC_initialise(void)
{
    TRISA = 0x03;   //Assign AN0 (RA0) and AN1 (RA1) as input  
    ADCON1bits.ADCS2 = 0;   //Select ADC conversion clock select as Fosc/8
    ADCON0bits.ADCS = 0x01; /*This provides the required minimum conversion time
                              with a 3.2768 MHz clock*/

    ADCON1bits.ADFM = 0x01; //Set results to right justified
    ADCON1bits.PCFG = 0x00; //Assign all ADC inputs as analogue  
    ADCON0bits.ADON = 1;    //Turn ADC On
}

unsigned int ADC_read(void)
{
    unsigned int result;
    __delay_us(20); //Wait for acquisition time (worst case 20 us)
    ADCON0bits.GO = 1;  //Set GO Bit to start conversion

    while(ADCON0bits.GO==1);    //Wait for GO bit to clear (conversion complete)
    result =(ADRESH<<8)+ADRESL; //Combine to produce final 10 bit result
    return(result);
}

void ADC_channel_select(unsigned char channel)
{
    ADCON0bits.CHS=channel; /*This selects which analogue input
                              to use for the ADC conversion*/
}