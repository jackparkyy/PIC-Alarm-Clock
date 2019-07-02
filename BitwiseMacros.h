/* Project:     AlarmClock_v1.3.2
 * File:        BitwiseMacros.h
 * Author:      Jack Parkinson
 * Date:        27/04/2017
 * Description: Defines macros to do bitwise maniplualtion on two 8-bit char
 *              variables used by mutliple files in the ALarmClock project*/

//Makes variables used in by macros avaible to external files
extern char modes1, modes2;

//Checks if the following macros have already been defined in included files
#ifndef BITWISE_MACROS_H
#define	BITWISE_MACROS_H

    //define bitwise macros for checking bits in modes1
    #define TEMP_MODE ((modes1 >> 0) & 1)
    #define ALARM_ON ((modes1 >> 1) & 1)
    #define RH_DISPLAYED ((modes1 >> 2) & 1)
    #define TEMP_DISPLAYED ((modes1 >> 3) & 1)
    #define SNOOZE_ENABLED ((modes1 >> 4) & 1)
    #define RB_PRESSED ((modes1 >> 5) & 1)
    #define ALARM_MODE ((modes1 >> 6) & 1)
    #define CONFIG_MODE ((modes1 >> 7) & 1)


    //define bitwise macros for checking bits in modes2
    #define RD0_PRESSED ((modes2 >> 0) & 1)
    #define RD1_PRESSED ((modes2 >> 1) & 1)
    #define RD2_PRESSED ((modes2 >> 2) & 1)
    #define RD3_PRESSED ((modes2 >> 3) & 1)
    #define RD4_PRESSED ((modes2 >> 4) & 1)
    #define TIME_MENU ((modes2 >> 5) & 1)
    #define ALARM_MENU ((modes2 >> 6) & 1)
    #define TEMP_MODE_MENU ((modes2 >> 7) & 1)


    //define bitwise macros for setting bits within modes1 to 1
    #define TEMP_MODE_CELSIUS (modes1 |= 0x01)
    #define ENABLE_ALARM_ON (modes1 |= 0x02)
    #define ENABLE_RH_DISPLAYED (modes1 |= 0x04)
    #define ENABLE_TEMP_DISPLAYED (modes1 |= 0x08)
    #define ENABLE_SNOOZE (modes1 |= 0x10)
    #define ENABLE_RB_PRESSED (modes1 |= 0x20)
    #define ENABLE_ALARM_MODE (modes1 |= 0x40)
    #define ENABLE_CONFIG_MODE (modes1 |= 0x80)

    //define bitwise macros for settings bits within modes1 to 0
    #define TEMP_MODE_FAHRENHEIT (modes1 &= 0xFE)
    #define DISABLE_ALARM_ON (modes1 &= 0xFD)
    #define DISABLE_RH_DISPLAYED (modes1 &= 0xFB)
    #define DISABLE_TEMP_DISPLAYED (modes1 &= 0xF7)
    #define DISABLE_SNOOZE (modes1 &= 0xEF)
    #define DISABLE_RB_PRESSED (modes1 &= 0xDF)
    #define DISABLE_ALARM_MODE (modes1 &= 0xBF)
    #define DISABLE_CONFIG_MODE (modes1 &= 0x7F)


    //define bitwise macros for setting bits within modes2 to 1
    #define ENABLE_RD0_PRESSED (modes2 |= 0x01)
    #define ENABLE_RD1_PRESSED (modes2 |= 0x02)
    #define ENABLE_RD2_PRESSED (modes2 |= 0x04)
    #define ENABLE_RD3_PRESSED (modes2 |= 0x08)
    #define ENABLE_RD4_PRESSED (modes2 |= 0x10)
    #define ENABLE_TIME_MENU (modes2 |= 0x20)
    #define ENABLE_ALARM_MENU (modes2 |= 0x40)
    #define ENABLE_TEMP_MODE_MENU (modes2 |= 0x80)

    //define bitwise macros for setting bits within modes2 to 0
    #define DISABLE_RD0_PRESSED (modes2 &= 0xFE)
    #define DISABLE_RD1_PRESSED (modes2 &= 0xFD)
    #define DISABLE_RD2_PRESSED (modes2 &= 0xFB)
    #define DISABLE_RD3_PRESSED (modes2 &= 0xF7)
    #define DISABLE_RD4_PRESSED (modes2 &= 0xEF)
    #define DISABLE_TIME_MENU (modes2 &= 0xDF)
    #define DISABLE_ALARM_MENU (modes2 &= 0xBF)
    #define DISABLE_TEMP_MODE_MENU (modes2 &= 0x7F)

    //define other common bitwise operations for modes1 and modes2
    #define DISABLE_PORTD_BUTTONS (modes2 &= 0xE0)
    #define RETURN_TO_MAIN_MENU (modes2 = 0)
    #define RETURN_TO_RUNNING_MODE modes2 = 0; modes1 &= 0x7F;

#endif