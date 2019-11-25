#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "../Model/Model.h"
#include "../View/View.h"
#include "raylib.h"

#define STATUS_START        0
#define STATUS_MENU         1
#define STATUS_PLAYING      2
#define STATUS_QUIT         3
#define STATUS_SETTINGS     4
#define STATUS_EDIT         5

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
  void doEditMenu(control_info gamepad, int level_start, std::string path);

  void testLevel();
  void generate_Level(std::string level);

  void createLevel();//experimental for now.
  void levelToTxt(std::vector<Entity> ent_list, int level_start, std::string file_path);
};

#endif
