/* Project:     AlarmClock_v1.3.2
 * File:        Display.h
 * Author:      Jack Parkinson
 * Date:        27/04/2017
 * Author:      Jack Parkinson
 * Description: Contains function prototypes for the ADC custom library to
                display the time, alarm time, humidity and temperature on the
                LCD*/

//Prints the alarm time in hh:mm on the bottom row of the LCD
void displayAlarm(unsigned long);

//Prints the humidity as a percentage on the bottom row of the LCD
void displayRH(void);

//Prints the temperature on the bottom row of the LCD in either degrees C or F
void displayTemp(char);

//Prints the time in hh:mm:ss on the top row of the LCD
void displayTime(unsigned long);