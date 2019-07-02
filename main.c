/* Project:     AlarmClock_v1.3.2
 * File:        main.c
 * Author:      Jack Parkinson
 * Date:        27/04/2017
 * Description: An alarm Clock program for the PIC16F877A*/

#include <xc.h> //Header file required for all MPLAB XC8 source files
#include "LCDdrive.h"   //Header file needed to access the LCD library
#include "ADC.h" //Header file needed to access the ADC library
#include "Display.h"    //Header file needed to access the Display library
#include "Menus.h"  //Header file needed to access the Menus library
#include "BitwiseMacros.h"  //Header file required for the custom Bitwise Macros

//Set clock speed of PIC16F877A to 3.2768MHz (Required by the __delay_ms macro.)
#define _XTAL_FREQ 3276800

/*Initialise elapsedTime to store number of half seconds elapsed
 *Initialise alarmTime to store the time the alarm is set to in half seconds*/
static unsigned long elapsedTime = 0, alarmTime = 57600;

//Initialise TMR0overflows (stores the number of Timer0 overflows)
static unsigned short TMR0overflows = 0;

//Function prototypes
void setup(void);
void checkAlarm(void);

//Main function
void main(void)
{
    unsigned long setTime = 0;

    setup();

    //Infinite loop so setup only runs at boot/reset
    while(1)
    {
        //While in running mode display useful information on LCD
        while(CONFIG_MODE == 0)
        {
            //Display the time
            displayTime(elapsedTime);

            //Display Alarm time, Relative Humidity and Temperature on LCD
            if(RH_DISPLAYED == 1)
                displayRH();
            else if(TEMP_DISPLAYED == 1)
                displayTemp(TEMP_MODE);
            else
                displayAlarm(alarmTime);

            //If RD2 is pressed go into config mode
            if(RD2_PRESSED == 1)
            {
                ENABLE_CONFIG_MODE;
            }

            checkAlarm();
        }

        LCD_clear(); /*Clear the LCD when switching between running and config
                       modes*/

        //While in config mode display menu system on LCD
        while(CONFIG_MODE == 1)
        {
            mainMenu();

            LCD_clear();
            if(TIME_MENU == 1)
            {
                setTime = setTimeMenu();
                if (setTime < 172801)
                    elapsedTime = setTime;
            }

            if(ALARM_MENU == 1)
            {
                setTime = setAlarmMenu();
                if (setTime < 172801)
                    alarmTime = setTime;
            }

            if(TEMP_MODE_MENU == 1)
                tempModeMenu();
        }
        while(RD2 == 1);
        DISABLE_PORTD_BUTTONS;
    }
}

//Setup PIC ready for use
void setup(void)
{
    INTCONbits.GIE = 0;     //Disable interrupts during configuration

    //Configure PORTB so RB7 is an input and the reset are outputs
    TRISB = 0x80;           
    PORTB = 0x00;

    //Configure PORTD so RD7, RD6 and RD5 are outputs and the reset are inputs
    TRISD = 0x1F;
    PORTD = 0x00;

    T1CON = 0x30;           //Timer1 OFF, Prescaler 1:8
    PR2 = 255;              //Load TIMER2 period register
    T2CON = 0x7E;           //Timer2 ON, Prescaler 1:16 Postscaler 1:8

    PIE1 = 0x03;            //Enable TIMER1 and TIMER2 interrupts ONLY
    
    PIR1bits.TMR1IF = 0;    //Clear Timer1 interrupt flag
    TMR1H = 0x38;           //Load high byte
    TMR1L = 0x00;           //Load low byte
    T1CONbits.TMR1ON = 1;   //Turn Timer 1 ON

    //Timer0 setup for 0.25s delay
    OPTION_REGbits.T0CS = 0;    //Set clock source to internal (timer mode)
    OPTION_REGbits.PSA = 0;     //Set prescaler to Timer 0
    OPTION_REGbits.PS = 7;      //Set prescaler bits to 111 for 1:256
    TMR0 = 96;                  //Preload Timer0

    INTCON = 0xE8;          //Enable GIE, PEIE and RBIE

    LCD_initialise();       //Initialise the LCD ready for use
    ADC_initialise();       //Initialise the ADC ready for use

    LCD_cursor(0,0);
    LCD_putsc("Time:");
}

void checkAlarm(void)
{
    //If RB7 has been pressed toggle alarm mode enabled/disabled
    if(RB_PRESSED == 1)
    {
        //If alarm mode is disabled, enable it
        if(ALARM_MODE == 0)
        {
            ENABLE_ALARM_MODE;
            PORTD |= 0x40; //enable D6
        }
        //If alarm mode is enabled, disable it
        else
        {
            DISABLE_ALARM_MODE;
            DISABLE_ALARM_ON; //Make sure the alarm isn't on
            DISABLE_PORTD_BUTTONS;
            PORTD &= 0xBF; //disable D6
            PORTD &= 0x7F; //disable D7
            TMR0overflows = 0;
        }
        __delay_ms(150);
        DISABLE_RB_PRESSED; //set RB to not pressed
    }
    /*If current time equals the time the alarm is set to and alarm mode is
      enabled turn the alarm on*/
    else if(elapsedTime == alarmTime && ALARM_MODE == 1)
    {
        ENABLE_ALARM_ON;
        DISABLE_RB_PRESSED;
    }
    //If RD4 has been pressed and the alarm is on, enable snooze
    else if(RD4_PRESSED == 1)
    {
        if(ALARM_ON == 1)
        {
            ENABLE_SNOOZE;
            PORTD &= 0x7F; //disable D7
            TMR0overflows = 0;
        }
        __delay_ms(150);
        DISABLE_PORTD_BUTTONS;
    }
}

//Interupt Service Routine
void interrupt myISR(void)
{
    //If half a second has passed (Timer1 has overflowed)
    if(PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;    //Clear Timer1 interrupt flag

        elapsedTime++;

        //If elapsedTime hits 24 hours, reset
        if(elapsedTime >= 172800)
            elapsedTime=0;
     
        T1CONbits.TMR1ON = 0;   //Turn Timer 1 OFF
        TMR1H = 0x38;           //Load high byte
        TMR1L = 0x00;           //Load low byte
        T1CONbits.TMR1ON = 1;   //Turn Timer 1 ON

        /*If 5 seconds has elapsed toggle what is displayed on LCD while in
         * running mode*/
        if((elapsedTime%10) == 0)
        {
            if(RH_DISPLAYED == 1)
            {
                ENABLE_TEMP_DISPLAYED;
                DISABLE_RH_DISPLAYED;
            }
            else if(TEMP_DISPLAYED == 1)
                DISABLE_TEMP_DISPLAYED;
            else
                ENABLE_RH_DISPLAYED;
        }
    }

    //Check RD buttons ever 0.03984375 seconds
    if(PIR1bits.TMR2IF)
    {
        if(RD0==1)
            ENABLE_RD0_PRESSED;
        else if(RD1==1)
            ENABLE_RD1_PRESSED;
        else if(RD2==1)
            ENABLE_RD2_PRESSED;
        else if(RD3==1)
            ENABLE_RD3_PRESSED;
        else if(RD4==1)
            ENABLE_RD4_PRESSED;

        PIR1bits.TMR2IF = 0;
    }

    /*If 0.05 seconds has passed (Timer0 has overflowed) and alarmOn is enabled
      flash LED D7 20 times and check for snooze*/
    if(INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;  //Clear Timer0 interrupt flag
 
        if(ALARM_ON == 1)
        {
            TMR0overflows++;

            //If snooze is enabled delay for 5 seconds before flashing 20 times
            if(SNOOZE_ENABLED == 1)
            {
                //If 20 seconds has passed, stop the alarm
                if(TMR0overflows > 404)
                {
                    TMR0overflows = 0;
                    DISABLE_ALARM_ON;
                    DISABLE_SNOOZE;
                    PORTD &= 0x7F; //Disable D7
                }
                //If 5 seconds has passed, flash LED D7
                else if(TMR0overflows > 99)
                {
                    //Enable LED D7 for 0.25seconds
                    if(TMR0overflows%15 == 0)
                        PORTD |= 0x80;
                    //Disable LED D7 for 0.5seconds
                    else if(TMR0overflows%5 == 0)
                        PORTD &= 0x7F;
                }
            }
            //If snooze is disabled flash LED D7 20 times
            else
            {
                //If 15 seconds has passed, stop the alarm
                if(TMR0overflows > 304)
                {
                    TMR0overflows = 0;
                    DISABLE_ALARM_ON;
                    PORTD &= 0x7F;
                }
                //flash LED D7
                else
                {
                    //Enable LED D7 for 0.5seconds
                    if(TMR0overflows%15 == 0)
                        PORTD |= 0x80;
                    //Disable LED D7 for 0.5seconds
                    else if(TMR0overflows%5 == 0)
                        PORTD &= 0x7F;
                }
            }
        }
        TMR0 = 96;  //Preload Timer0
    }

    if(INTCONbits.RBIF)
    {
        INTCONbits.RBIF = 0;    //Clear RB interrupt flag
        ENABLE_RB_PRESSED; //Set RBPressed bit within modes2 to 1
    }
}