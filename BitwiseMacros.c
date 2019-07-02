/*modes1 (variables to store 8 booleans):
 * bit0 = tempMode (1=degrees C, 0=degrees F)
 * bit1 = alarmOn (1=on, 0=off)
 * bit2 = rhDisplayed (1=displayed, 0=not displayed)
 * bit3 = tempDisplayed (1=displayed, 0=not displayed)
 * bit4 = Snooze (1=enabled, 0=disabled)
 * bit5 = RBPressed (1=true, 0=false)
 * bit6 = alarmMode (1=enabled, 0=disabled)
 * bit7 = configMode (1=enabled, 0=disabled)
 *
 * modes2 (variables to store 8 booleans):
 * bit0 = RD0Pressed (1=true, 0=false)
 * bit1 = RD1Pressed (1=true, 0=false)
 * bit2 = RD2Pressed (1=true, 0=false)
 * bit3 = RD3Pressed (1=true, 0=false)
 * bit4 = RD4Pressed (1=true, 0=false)
 * bit5 = setTimeMenu (1=displayed, 0=not displayed)
 * bit6 = setAlarmMenu (1=displayed, 0=not displayed)
 * bit7 = tempModeMenu (1=displayed, 0=not displayed)*/
unsigned char modes1 = 0x01, modes2 = 0;