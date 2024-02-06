/******************************************************************************
* This is only a wrapper class for screen and hold the graphical objects
* To use a different library simply override the screen
* Licence: AGPL3
* Author: S. Fambach
* Visit http://www.fambach.net if you want
* Libs:
* https://github.com/DFRobot/DFRobot_OLED12864
* https://github.com/sfambach/CustomKeyboard/
* 
******************************************************************************/

#ifndef Screen_h
#define Screen_h


class Screen {

  enum Error {
    OK = 0,
    ArrayFull = 1
  };

  GraphicalObject* _graphicalObjects[];
  int _objectCount = 0;
  const int _maxObjectCount;

public:

  Screen(int maxObjectCount)
    : _maxObjectCount(maxObjectCount) {
    _graphicalObjects = new GraphicalObject[maxObjectCount];
  }


  virtual ~Screen() {
    // delete all objects
  }

  Error add(GraphicalObject* go) {
    if (isFull) {
      // array full
      return Error::ArrayFull;
    }

    _graphicalObjects[_objectCount] = go : _objectCount++;
  }

  void draw() {

    // loop over all object and draw them
    for (int i = 0; i < _objectCount; i++) {
      if (_graphicalObject[i] == NULL) {
        continue;
      }

      if (!_graphicalObject[i]->isVisible()) {
        continue;
      }

      _graphicalObject[i]->draw(this);
    }
  }

  bool isFull() {
    return _objectCount == _maxObjectCount - 1;
  }


  /*******************************************************************************/
  // Wrapper functions

  virtual void drawRect(int posX, int posY, int width, int height, int color) = 0;
  virtual void fillRect(int posX, int posY, int width, int height, int color) = 0;
  virtual void setColor(OLEDDISPLAY_COLOR color) = 0;
  virtual void setPixel(int16_t x, int16_t y) = 0;
  virtual void clearPixel(int16_t x, int16_t y) = 0;
  virtual virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) = 0;
  virtual void drawCircle(int16_t x, int16_t y, int16_t radius) = 0;
  virtual void drawCircleQuads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads) = 0;
  virtual void fillCircle(int16_t x, int16_t y, int16_t radius) = 0;
  virtual void drawFastImage(int16_t x, int16_t y, int16_t width, int16_t height, const char* image) = 0;
  virtual void drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const char* xbm) = 0;
  virtual void drawString(int16_t x, int16_t y, String text) = 0;
  virtual void drawStringMaxWidth(int16_t x, int16_t y, uint16_t maxLineWidth, String text) = 0;
  virtual uint16_t getStringWidth(const char* text, uint16_t length) = 0;
  virtual uint16_t getStringWidth(String text) = 0;
  virtual void setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT textAlignment) = 0;
  virtual void setFont(const char* fontData) = 0;
  virtual void flipScreenVertically(void) = 0;
  // this others will follow or not ;)

  /*******************************************************************************/
};


#endif  // Screen_h