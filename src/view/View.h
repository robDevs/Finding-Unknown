#ifndef _VIEW_
#define _VIEW_

#include "raylib.h"

#include <string>

//-------------------------------------------------------------------------------------------
// Control info struct. 
// Allows the controller (controller.h) not worry about the button and only the action. 
//-------------------------------------------------------------------------------------------
struct control_info {
  void init();
  bool up_pressed;
  bool up_held;
  bool up_released;

  bool down_pressed;
  bool down_held;
  bool down_released;

  bool left_pressed;
  bool left_held;
  bool left_released;

  bool right_pressed;
  bool right_held;
  bool right_released;
};

//------------------------------------------------------------------------------------------
// The view handles drawing things and getting input from user. 
//------------------------------------------------------------------------------------------
class View {
private:
  int screenWidth;
  int screenHeight;
public:
  void initView(int width, int height); // Create a window with the given attributes. 
  bool getWindowStatus(); // check if the window should close (If x thing was pressed. )
  void drawTexture(int x, int y, Texture2D texture); // Draws a texture at the given pos. 
  void drawText(std::string text, int x, int y, int size, Color color); // draws text at given pos. 
  control_info getControlInfo(); // read input and return a control_info struct. 
};

#endif
