/******************************************************************************
* Object oriened version of "the Draw and move a rectangle on the screen" program
* Licence: AGPL3
* Author: S. Fambach
* Visit http://www.fambach.net if you want
* Libs:
* https://github.com/DFRobot/DFRobot_OLED12864
* https://github.com/sfambach/CustomKeyboard/
* 
******************************************************************************/
#include <CustomKeyboard.h> // https://github.com/sfambach/CustomKeyboard/
#include "DFRobot_OLED12864.h"  // https://github.com/DFRobot/DFRobot_OLED12864


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


void displaySetup(){
 display.init();
 display.flipScreenVertically();
 
 display.setContrast(200);
 display.clear();
}

/** animation *****************************************************************/
#define RECT_WIDTH 5
#define RECT_HEIGHT 5
int posX = 10;
int posY = 10;
int posXOld = 10;
int posYOld = 10;

#define COLLISION_NOTHING 0
#define COLLISION_STOP_AT_WALL 1
#define COLLISION_ENDLESS 2

int collision = COLLISION_STOP_AT_WALL;
//bool crash = false;
void updateRect(){

  if ( collision == COLLISION_STOP_AT_WALL ){
      if(posX < 0 || posX >DISPLAY_WIDTH ){
        posX = posXOld;

      } else if(posY < 0 || posY > DISPLAY_HEIGHT){
        posY = posYOld;
      }
  }


  display.setColor(OLEDDISPLAY_COLOR::BLACK);
  display.fillRect(posXOld, posYOld, RECT_WIDTH, RECT_HEIGHT);
  display.setColor(OLEDDISPLAY_COLOR::WHITE );
  posXOld = posX;
  posYOld = posY;
  display.fillRect(posXOld, posYOld, RECT_WIDTH, RECT_HEIGHT);
  display.drawRect(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);
  displayRefresh();
}

/** Button setup *************************************************************/


CustomKeyboard keyboard(3);

void keyboardOutput(char* name, int state){
  DEBUG_PRINT(name);
  DEBUG_PRINT(" is ");
  DEBUG_PRINTLN(state);
}

void keyboardGP1(AbstractKeyboardElement* element, int state){
  //keyboardOutput("Gamepad", state);
  if(state > 33 && state < 44 ){
    keyboardOutput("Gamepad up", state);
    posY-=RECT_HEIGHT;
    updateRect();
  }
  
  if(state >= 0 && state < 15){
    keyboardOutput("Gamepad down", state);
    posY+=RECT_HEIGHT;
    updateRect();
  }

  if(state > 15 && state < 23 ){
    keyboardOutput("Gamepad left", state);
    posX-=RECT_WIDTH;
    updateRect();
  }
  
  if(state > 72 && state < 80 ){
    keyboardOutput("Gamepad right", state);
    posX+=RECT_WIDTH;
    updateRect();
  }
  
  if(state > 54 && state < 62 ){
    keyboardOutput("Gamepad fire", state);
  }
  
}

void keyboardB1(AbstractKeyboardElement* element, int state){
  keyboardOutput("B1", state);

}
void keyboardB2(AbstractKeyboardElement* element, int state){
  keyboardOutput("B2", state);

}

void keyboardSetup(){

  Poti* pot = new Poti(A0, keyboardGP1);
  pot->setThreshold(5);
  keyboard.addElement(pot);
  keyboard.addElement(new Button(D3, keyboardB1));
  keyboard.addElement(new Button(D8, keyboardB2));
  keyboard.setup();
}

void displayRefresh(){
    display.display();
    delay(10);
}




/** XXXXXXXX *****************************************************************/
/** XXXXXXXX *****************************************************************/


/** Main Programm ************************************************************/
void setup() {
  Serial.begin(115200);
  DEBUG_PRINTLN("Setup");

  DEBUG_PRINTLN("Init the keyboard");
  keyboardSetup();

  DEBUG_PRINTLN("Init the display");
  displaySetup();
  DEBUG_PRINTLN("Next");
  
  displayRefresh();
  updateRect();


  
}


void loop() {
  //DEBUG_PRINTLN("Main Loop");
    keyboard.loop();
}

void serialEvent(){

}

/** Rest of implementations **************************************************/