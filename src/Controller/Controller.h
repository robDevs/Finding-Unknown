#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <vector>

#include "../Model/Model.h"
#include "../View/View.h"

#define STATUS_START        0
#define STATUS_MENU         1
#define STATUS_PLAYING      2
#define STATUS_QUIT         3
#define STATUS_SETTINGS     4

class Controller {
private:
  std::vector<Entity> enemies;
  View view;
  int gamestatus;
  float xScale, yScale;

  Entity player;
public:
  Controller();
  void entityLoop();
  void doIntro();
  void gameLoop();
  void doMenu();
  void doSettings();
  void doGame();
};

#endif
