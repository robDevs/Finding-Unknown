#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <vector>

#include "../Model/Model.h"
#include "../View/View.h"

#define STATUS_MENU         0
#define STATUS_PLAYING      1

class Controller {
private:
  std::vector<Entity> entities;
  View view;
  int gamestatus;

public:
  Controller();
  void entityLoop();
  void gameLoop();
  void doMenu();
  void doGame();
};

#endif
