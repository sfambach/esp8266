/******************************************************************************
* Draw a rectangle on the screen
* Licence: AGPL3
* Author: S. Fambach
* Visit http://www.fambach.net if you want
* Libs:
* https://github.com/DFRobot/DFRobot_OLED12864
******************************************************************************/


/** Debuging *****************************************************************/
#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif


/** Display setup*************************************************************/
// For a connection via I2C using Wire include
#include "DFRobot_OLED12864.h"  // https://github.com/DFRobot/DFRobot_OLED12864

DFRobot_OLED12864  display(0x3c);

#define RECT_WIDTH 5
#define RECT_HEIGHT 5


void displaySetup(){
 display.init();
 // display.flipScreenVertically();
 // display.flipScreenHorizontally();
 display.setContrast(200);
 display.clear();
}

void displayDrawRect(int16_t posx, int16_t posy){
  display.drawRect(posx, posy, RECT_WIDTH, RECT_HEIGHT);
}

void displayFillRect(int16_t posx, int16_t posy){
  display.fillRect( posx,  posy, RECT_WIDTH, RECT_HEIGHT);
}


void displayRefresh(){
    display.display();
    delay(10);
}

/** Main Programm ************************************************************/
void setup() {
  Serial.begin(115200);
  DEBUG_PRINTLN("Setup");

  displaySetup();
  displayFillRect(10,10);
  displayRefresh();
}


void loop() {
  //DEBUG_PRINTLN("Main Loop");
}

void serialEvent(){
  DEBUG_PRINTLN("Serial Event");
}

/** Rest of implementations **************************************************/