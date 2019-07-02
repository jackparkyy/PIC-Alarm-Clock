/* Project:     AlarmClock_v1.3.2
 * File:        Menus.c
 * Author:      Jack Parkinson
 * Date:        27/04/2017
 * Description: Displays an interactive menu system on the LCD which allows the
                user to set the time, alarm time and temp mode*/

#include <xc.h> // Header file required for all MPLAB XC8 source files
#include "LCDdrive.h"   //Header file needed to access to LCD custom library
#include "BitwiseMacros.h"  //Header file required for the custom Bitwise Macros

//Set clock speed of PIC16F877A to 3.2768MHz (Required by the __delay_ms macro.)
#define _XTAL_FREQ 3276800

//Function prototypes
unsigned long displaySetTime(void);

void mainMenu(void)
{
    unsigned char menuItem = 0;
    while(RD2 == 1);    //wait for user to let go of RD2
    while(RD3 == 1);    //wait for user to let go of RD3
    DISABLE_PORTD_BUTTONS;

    /*While in main menu continuously check for button presses, change what
      is displayed on the LCD accordingly and allow the user to select a sub
      menu*/
    while(TIME_MENU == 0 && ALARM_MENU == 0 &&
            TEMP_MODE_MENU == 0 && CONFIG_MODE == 1)
    {
        //Change LCD according to menuItem
        switch(menuItem)
        {
            case 0:
                LCD_cursor(0,0);
                LCD_putsc("*Set Time       ");
                LCD_cursor(0,1);
                LCD_putsc(" Set Alarm      ");
                break;
            case 1:
                LCD_cursor(0,0);
                LCD_putsc(" Set Time       ");
                LCD_cursor(0,1);
                LCD_putsc("*Set Alarm      ");
                break;
            case 2:
                LCD_cursor(0,0);
                LCD_putsc("*Set Alarm      ");
                LCD_cursor(0,1);
                LCD_putsc(" Temp Mode      ");
                break;
            case 3:
                LCD_cursor(0,0);
                LCD_putsc(" Set Alarm      ");
                LCD_cursor(0,1);
                LCD_putsc("*Temp Mode      ");
                break;
        }

        //If RD0 has been pressed and not at upper bound of menu scroll up
        if(RD0_PRESSED == 1 && menuItem != 0)
        {
            if(menuItem == 2)
                menuItem -= 2;

            else
                menuItem--;

            __delay_ms(150);    //Switch debounce delay
        }
        //If RD1 has been pressed and not at lower bound scroll down
        else if(RD1_PRESSED == 1 && menuItem != 3)
        {
            if(menuItem == 1)
                menuItem += 2;

            else
                menuItem++;
            
            __delay_ms(150);    //Switch debounce delay
        }
        //If RD2 has been pressed go to selected menu
        else if(RD2_PRESSED == 1)
        {
            switch(menuItem)
            {
                case 0:
                    ENABLE_TIME_MENU;
                    break;
                case 3:
                    ENABLE_TEMP_MODE_MENU;
                    break;
                default:
                    ENABLE_ALARM_MENU;
                    break;
            }
        }
        //If RD3 has been pressed go back to running mode
        else if(RD3_PRESSED == 1)
        {
            RETURN_TO_RUNNING_MODE;
        }
        DISABLE_PORTD_BUTTONS;
    }
}

unsigned long setTimeMenu(void)
{
    unsigned long setTime = 0;

    while(RD2 == 1);    //wait for user to let go of RD2
    DISABLE_PORTD_BUTTONS;

    LCD_cursor(0,0);
    LCD_putsc("Set Time        ");

    setTime = displaySetTime(); //Call the displaySetTime function

    return(setTime);
}

unsigned long setAlarmMenu(void)
{
    unsigned long setTime = 0;

    while(RD2 == 1);    //wait for user to let go of RD2
    DISABLE_PORTD_BUTTONS;

    LCD_cursor(0,0);
    LCD_putsc("Set Alarm Time  ");
    
    setTime = displaySetTime(); //Call the displaySetTime function
    
    return(setTime);
}

void tempModeMenu(void)
{
    while(RD2 == 1);    //Wait for user to let go of RD2
    DISABLE_PORTD_BUTTONS; //Set all buttons to not pressed in modes2
    
    /*While in temp mode menu continuously check for button presses, change what
      is displayed on the LCD accordingly and allow the user to select a temp
      mode (degrees C or F)*/
    while(TEMP_MODE_MENU == 1)
    {
        char menuItem;
        //Chnage LCD accorinding to menuItem
        switch(menuItem)
        {
            case 1:
                LCD_cursor(0,0);
                LCD_putsc(" Degrees C");
                LCD_cursor(0,1);
                LCD_putsc("*Degrees F");
                break;
            default:
                LCD_cursor(0,0);
                LCD_putsc("*Degrees C");
                LCD_cursor(0,1);
                LCD_putsc(" Degrees F");
                menuItem = 0;
                break;
        }
    
        //If RD0 has been pressed and not at upper bound of menu scroll up
        if(RD0_PRESSED == 1 && menuItem != 0)
        {
            menuItem--;
            __delay_ms(150);
        }
        //If RD1 has been pressed and not at lower bound scroll down
        else if(RD1_PRESSED == 1 && menuItem != 1)
        {
            menuItem++;
            __delay_ms(150);
        }

        /*If RD2 has been pressed set the temp mode according to menu item and
          return to running mode*/
        else if(RD2_PRESSED == 1)
        {
            switch(menuItem)
            {
                case 1:
                    TEMP_MODE_FAHRENHEIT; //Set tempMode bit to 0
                    break;
                default:
                    TEMP_MODE_CELSIUS; //Set tempMode bit to 1
                    break;
            }
            RETURN_TO_RUNNING_MODE;
        }
        //If RD3 has been pressed go back to main mode
        else if(RD3_PRESSED == 1)
        {
            RETURN_TO_MAIN_MENU;
        }
        DISABLE_PORTD_BUTTONS;
    }
}

unsigned long displaySetTime(void)
{
    unsigned long setTime = 172801;
    unsigned char selectedDigit = 0;
    unsigned char bounds[4] = {2,9,5,9};
    unsigned char digits[4] = {0,0,0,0};

    LCD_cursor(0,1);
    LCD_putsc("__:__");
    
    while(selectedDigit<4)
    {
        /*Alter the bounds of the second hour digit based on the first hour
          digit to prevent the user from entering a time greater than 23:59*/
        if(digits[0] == 2)
        {
            bounds[1] = 3;
            if(digits[1] > bounds[1])
            {
                digits[1] = 0;
                LCD_cursor(1,1);
                LCD_display_value(0);
            }
        }
        else
            bounds[1] = 9;

        LCD_cursor(selectedDigit,1);

        /*if the selected digit is a minutes digit, move the cursor on by one
          column to account for the : in the hh:mm format*/
        if(selectedDigit>1)
            LCD_cursor(selectedDigit+1,1);

        /*Enable the flashing cursor to indicate which digit is selected unless
          the user is holding down either RD0 to RD1, to allow them to scroll
          faster*/
        if(RD0 == 0 && RD1 == 0)
        {
            LCD_cursor_on();
            __delay_ms(190);
            LCD_cursor_off();
        }
        
        LCD_display_value(digits[selectedDigit]);
        
        //If RD0 has been pressed and not at upper bound of digit scroll up
        if(RD0_PRESSED == 1 && digits[selectedDigit] != bounds[selectedDigit])
        {
            digits[selectedDigit]++;
            __delay_ms(100);    //switch debounce delay
            DISABLE_PORTD_BUTTONS; //Set all buttons to not pressed in modes2
        }
        
        //If RD1 has been pressed and not at lower bound of digit scroll down
        else if(RD1_PRESSED == 1 &&  digits[selectedDigit] != 0)
        {
            digits[selectedDigit]--;
            __delay_ms(100);    //switch debounce delay
            DISABLE_PORTD_BUTTONS; //Set all buttons to not pressed in modes2
        }

        /*If RD2 has been pressed move to next digit unless on the last digit,
          in which case set the time*/
        else if(RD2_PRESSED == 1)
        {
            switch(selectedDigit)
            {
                //If on the last digit, return setTime in half seconds
                case 3:
                    setTime = (digits[0]*10)+digits[1];
                    setTime = setTime*7200 + (((digits[2]*10)+digits[3])*120);

                    RETURN_TO_RUNNING_MODE;
                    return(setTime);
                default:
                    selectedDigit++;
                    break;
            }
            while(RD2 == 1);
            DISABLE_PORTD_BUTTONS; //Set all buttons to not pressed in modes2
        }

        //If RD3 has been pressed go move cursor back or go to main menu
        else if(RD3_PRESSED == 1)
        {
            //switch(selectedDigit)
            switch(selectedDigit)
            {
                case 0:
                    RETURN_TO_MAIN_MENU;
                    selectedDigit=4;
                    break;
                default:
                    selectedDigit--;
                    break;
            }
            while(RD3 == 1);
            DISABLE_PORTD_BUTTONS; //Set all buttons to not pressed in modes2
        }
    }
    return(setTime);
}