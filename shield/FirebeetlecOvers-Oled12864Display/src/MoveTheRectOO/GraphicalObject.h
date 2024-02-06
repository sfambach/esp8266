#ifndef GraphicalObject_h
#define GraphicalObject_h

#includ "Screen.h"

class GraphicalObject {

  int _width;
  int _height;
  int _posX;
  int _posY;
  int _color;

  bool _visible;
  bool _moving = false;



  public:
  /** default consructor
  */
  GraphicalObject(int posX,int posY, int width, int height, int color = 0)
  :_posX(posX),_posY(posY), _width(width), _height(height), _color(color) { }
  
  
  /** draw this go on the screen 
  * s = a valid screen object pointer (!= NULL)
  */
  virtual void draw(Screen* s) = 0;

  /** check if there is a collision with another object
  * go = another graphical objects
  */
  virtual bool collision(GraphicalObject* go )= 0;

  /** This funcion is for animation it can be called in recurrend time ranges
  * to animate objects eg. for a game. I would define the time span for calling outside once, 
  * instead of checking the interval inside the object but there might be cirecdumstnace where this is not suffient.
  * then i would double the general speed and for slow movement I would update 
  * every time and for slowers i would update very second time
  */
  virtual void animate(){}

  void setMoving(bool moving){
    _moving = moving;
  }

  bool isMoving(){
    return _moving;
  }

};


#endif //GraphicalObject_h