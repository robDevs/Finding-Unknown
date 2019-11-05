#ifndef _VIEW_
#define _VIEW_

#include "raylib.h"

#include <string>


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

class View {
private:
  int screenWidth;
  int screenHeight;
public:
  void initView(int width, int height);
  bool getWindowStatus();
  void drawTexture(int x, int y, Texture2D texture);
  void drawText(std::string text, int x, int y, int size, Color color);
  control_info getControlInfo();
};

#endif
