#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <vector>

#include "../Model/Model.h"
#include "../View/View.h"
#include "raylib.h"

#define STATUS_START        0
#define STATUS_MENU         1
#define STATUS_PLAYING      2
#define STATUS_QUIT         3
#define STATUS_SETTINGS     4

class Controller {
private:
  std::vector<Entity> enemies;
  std::vector<Entity> bullets;
  std::vector<Entity> enemy_bullets;
  std::vector<Entity> explosions;
  View view;
  int gamestatus;
  float xScale, yScale;
  int points;
  bool pause;
  int pause_menu_pos;

  Entity player;
public:
  Controller();
  void entityLoop();
  void doIntro();
  void gameLoop();
  void doMenu();
  void doSettings();
  void doGame();
  void doPauseMenu(control_info gamepad);

  void testLevel();
};

#endif
