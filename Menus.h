/* Project:     AlarmClock_v1.3.2
 * File:        Menus.h
 * Author:      Jack Parkinson
 * Date:        27/04/2017
 * Author:      Jack Parkinson
 * Description: Contains function prototypes for the Menus custom library to
                display an interactive menu system comprised of a main menu and
                three sub menus for setting the time, alarm time and temp mode*/

/*Displays an interactive menu on the LCD which allows the user to select
  between the three sub menus*/
void mainMenu(void);

//Displays an interactive menu on the LCD which allows the user to set the time
unsigned long setTimeMenu(void);

/*Displays an interactive menu on the LCD which allows the user to set the
  alarm time*/
unsigned long setAlarmMenu(void);

/*Displays an interactive menu on the LCD which allows the user to choose to
  display the temperature in degrees C or F*/
void tempModeMenu(void);