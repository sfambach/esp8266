/** 
* Screen Implementation for the df robot library 
*
**/

#ifndef DFDRobotLED12864Screen_h
#define DFDRobotLED12864Screen_h
#include "DFRobot_OLED12864.h"

#define DISPLAY_HEIGHT 64

class DFRScreen: Screen{


protected:
  DFRobot_OLED12864  _display;
public:
  DFRScreen (){
    _display = DFRobot_OLED12864(0x3c);

  }


  virtual void drawRect(int posX, int posY, int width, int height, int color) {
    int color = _display.get
    _display.drawRect(posX,posY,width,height);
  }
  virtual void fillRect(int posX, int posY, int width, int height, int color){}
  virtual void setColor(OLEDDISPLAY_COLOR color){}
  virtual void setPixel(int16_t x, int16_t y){}
  virtual void clearPixel(int16_t x, int16_t y){}
  virtual virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1){}
  virtual void drawCircle(int16_t x, int16_t y, int16_t radius){}
  virtual void drawCircleQuads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads){}
  virtual void fillCircle(int16_t x, int16_t y, int16_t radius){}
  virtual void drawFastImage(int16_t x, int16_t y, int16_t width, int16_t height, const char* image){}
  virtual void drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const char* xbm){}
  virtual void drawString(int16_t x, int16_t y, String text){}
  virtual void drawStringMaxWidth(int16_t x, int16_t y, uint16_t maxLineWidth, String text){}
  virtual uint16_t getStringWidth(const char* text, uint16_t length){}
  virtual uint16_t getStringWidth(String text){}
  virtual void setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT textAlignment){}
  virtual void setFont(const char* fontData){}
  virtual void flipScreenVertically(void){}

}


#endif // DFDRobotLED12864Screen_h