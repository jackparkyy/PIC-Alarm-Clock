/* Project:     AlarmClock_v1.3.2
 * File:        Display.c
 * Author:      Jack Parkinson
 * Date:        27/04/2017
 * Description: Prints time, alarm time, humidity and temperature on the
                LCD*/

#include <xc.h> //Header file required for all MPLAB XC8 source files
#include "LCDdrive.h"   //Header file needed to access to LCD custom library
#include "ADC.h"    //Header file needed to access to ADC custom library

#define CELSIUS 1

void displayTime(unsigned long elapsedTime)
{
    char secs = 0;
    char mins = 0;
    char hours = 0;

    //Convert half seconds to seconds, minutes and hours
    secs = (elapsedTime/2)%60;
    mins = (elapsedTime%7200)/120;
    hours = elapsedTime/7200;

    LCD_cursor(0,0);
    LCD_putsc("Time: ");

    //Display routine for displaying the time
    if(hours<10)
        LCD_putch('0'); //Add a zero before single digits
    LCD_display_value(hours);

    LCD_putch(':');

    if(mins<10)
        LCD_putch('0'); //Add a zero before single digits
    LCD_display_value(mins);

    LCD_putch(':');

    if(secs<10)
        LCD_putch('0'); //Add a zero before single digits
    LCD_display_value(secs);
}

void displayAlarm(unsigned long alarmTime)
{
    char mins = 0;  //Stores the minutes
    char hours = 0; //Stores the hors

    //Convert half seconds to minutes and hours
    mins = (alarmTime%7200)/120;
    hours = alarmTime/7200;

    LCD_cursor(0,1);
    LCD_putsc("Alarm: ");

    //Display routine for displaying the alarm time
    if(hours<10)
        LCD_putch('0'); //Add a zero before single digits
    LCD_display_value(hours);

    LCD_putch(':');

    if(mins<10)
        LCD_putch('0'); //Add a zero before single digits
    LCD_display_value(mins);

    LCD_putsc("     "); //Clear the rest of the bottom row of the LCD
}

void displayRH(void)
{
    LCD_cursor(0,1);
    LCD_putsc("RH: ");

    //Select the ADC channel connected to humidity sensor
    ADC_channel_select(0);

    /*Convert the 10-bit integer returend by the ADC_read() function to a
      percentage between 10-90%*/
    LCD_cursor(4,1);
    LCD_display_float((ADC_read()*0.07820136852)+10,1);

    //Display units and clear the rest of the bottom row of the LCD
    LCD_cursor(8,1);
    LCD_putsc("%       ");
}

void displayTemp(char tempMode)
{
    LCD_cursor(0,1);
    LCD_putsc("Temp: ");

    //Select the ADC channel connected to temperature sensor
    ADC_channel_select(1);

    LCD_cursor(6,1);

    if(tempMode == CELSIUS)
    {
        //if ADC_read() > -10 degrees C < 100.0 degress C
        if(ADC_read() > 341 && ADC_read() < 1593)
        {
            LCD_putch(' ');
            //if ADC_read() > -0.1 degrees C < 10.0 degress C
            if(ADC_read() > 454 && ADC_read() < 569)
                LCD_putch(' ');
        }

        /*Convert the 10-bit integer returned by the ADC_read() function to a
          temperature between 50--40 degrees C*/
        LCD_display_float((ADC_read()*0.08797653959)-40,1);

        //Display units and clear the rest of the bottom row of the LCD
        LCD_cursor(11,1);
        LCD_putsc("degC ");
    }
    else
    {
        //if ADC_read() > -10 degrees F < 100.0 degress F
        if(ADC_read() > 190 && ADC_read() < 885)
        {
            LCD_putch(' ');
            //if ADC_read() > -0.1 degrees F < 10.0 degress F
            if(ADC_read() > 252 && ADC_read() < 316)
                LCD_putch(' ');
        }

        /*Convert the 10-bit integer returned by the ADC_read() function to a
          temperature between 122--40 degrees F*/
        LCD_display_float((ADC_read()*0.1583577713)-40,1);

        //Display units and clear the rest of the bottom row of the LCD
        LCD_cursor(11,1);
        LCD_putsc("degF ");
    }
}