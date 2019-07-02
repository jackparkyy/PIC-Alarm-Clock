/* Project:     AlarmClock_v1.3.2
 * File:        ADC.h
 * Author:      Jack Parkinson
 * Date:        27/04/2017
 * Description: Contains function prototypes for the ADC custom library to
                read Temperature and Humidity sensors*/

//Initialise the ADC ready for use
void ADC_initialise(void);

//Convers an analogue reading to a 10-bit digital value and returns that value
unsigned int ADC_read(void);

//Selects the ADC channel (0 = Humidity, 1 = Temperature)
void ADC_channel_select(unsigned char channel);