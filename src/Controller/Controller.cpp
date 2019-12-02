#include "Controller.h"

Controller::Controller() {
  gamestatus = STATUS_START;
  xScale = 1;
  yScale = 1;
  points = 0;
  pause = false;
  pause_menu_pos = 0;

  view.initView(1280,720, &xScale, &yScale);

  gameLoop();
}

void Controller::entityLoop() {
  player_pointer = &player;
  if(player.update != NULL) player.update();

  for(int i = 0; i < (int) enemies.size(); i++) {
    self = &enemies[i];
    if(enemies[i].update != NULL) enemies[i].update();
    if(enemies[i].shoot != NULL && enemies[i].status == ENTITY_SHOOT) {
      enemies[i].shoot(&enemy_bullets);
    }

    for(int x = 0; x < bullets.size(); x++) {
      other = &bullets[x];
      if(CheckCollisionRecs(enemies[i].rect, bullets[x].rect)) {
        if(enemies[i].hit != NULL) enemies[i].hit();
      }
    }

    if(enemies[i].status == ENTITY_REMOVE) {
      enemies.erase(enemies.begin() + i);
      //spawn_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), xScale, yScale, &enemies);
    }
    if(enemies[i].status == ENTITY_DESTROY) {
      spawn_explosion(enemies[i].rect.x + enemies[i].rect.width/2 - 200*xScale, enemies[i].rect.y + enemies[i].rect.width/2 - 200*yScale, 0, &explosions);

      points += enemies[i].points;
      enemies.erase(enemies.begin() + i);
      //spawn_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), xScale, yScale,  &enemies);
    }
  }

  for(int i = 0; i < (int) bullets.size(); i++) {
    self = &bullets[i];
    if(bullets[i].update != NULL) bullets[i].update();

    if(bullets[i].status == ENTITY_REMOVE) {
      bullets.erase(bullets.begin() + i);
    }
    if(bullets[i].status == ENTITY_DESTROY) {
      spawn_explosion(bullets[i].rect.x + bullets[i].rect.width/2 - 25*xScale, bullets[i].rect.y + bullets[i].rect.width/2 - 25*yScale, 1, &explosions);
      bullets.erase(bullets.begin() + i);
    }
  }

  for(int i = 0; i < (int) enemy_bullets.size(); i++) {
    self = &enemy_bullets[i];
    if(enemy_bullets[i].update != NULL) enemy_bullets[i].update();

    if(enemy_bullets[i].status == ENTITY_REMOVE) {
      enemy_bullets.erase(enemy_bullets.begin() + i);
    }
    if(enemy_bullets[i].status == ENTITY_DESTROY) {
      spawn_explosion(enemy_bullets[i].rect.x + enemy_bullets[i].rect.width/2 - 25*xScale, enemy_bullets[i].rect.y + enemy_bullets[i].rect.width/2 - 25*yScale, 1, &explosions);
      enemy_bullets.erase(enemy_bullets.begin() + i);
    }
  }

  for(int i = 0; i < (int) explosions.size(); i++) {
    self = &explosions[i];
    if(explosions[i].update != NULL) explosions[i].update();

    if(explosions[i].status == ENTITY_REMOVE) {
      explosions.erase(explosions.begin() + i);
    }
  }
}

void Controller::gameLoop() {
  while(!view.getWindowStatus() && gamestatus != STATUS_QUIT) {
    //doIntro(); //un-comment to test sprites after re-scaling.
    if(gamestatus == STATUS_START) {
      doIntro();
    }
    if(gamestatus == STATUS_MENU) {
      doMenu();
    }
    if(gamestatus == STATUS_PLAYING) {
      doGame();
    }
    if(gamestatus == STATUS_SETTINGS) {
      doSettings();
    }
    if(gamestatus == STATUS_EDIT) {
      createLevel();
    }
    if(gamestatus == STATUS_LEVEL_SEL) {
      levelSelect();
    }
  }
  view.freeTextures();
}

void Controller::doIntro() {
    int frameBeam = 0;
    int frameStars = 0;
    int frameBurst = 0;
    int frameTicker = 0;
    float beamPos = 0.00 - view.getTextureWidth(BEAM_A);
    int titlePos = view.getScreenHeight()/2 - view.getTextureHeight(TITLE_)/2;
    int titleFade = 0;
    while(1) {

      control_info gamepad = view.getControlInfo();

      if(gamepad.enter_released) {
        break;
      }

      //update things
      frameTicker++;
      if(frameTicker > 5) {
          frameTicker = 0;
          frameBeam += 1;
          frameStars += 1;
          if(beamPos + view.getTextureWidth(BEAM_A) > view.getScreenWidth()) frameBurst += 1;
          if(frameBeam > 1) frameBeam = 0;
          if(frameStars > 12) frameStars = 0;
      }

      beamPos += 20.0*xScale;

      if(beamPos + view.getTextureWidth(BEAM_A) > view.getScreenWidth() && titleFade < 255) {
          titleFade+=5;
          if(titlePos > 50*yScale) {
              titlePos -= 5*yScale;
          }
      }

      if(titleFade >= 255 && frameStars == 1) {
          break;
      }


      view.startFrame();

      if(view.getWindowStatus()) break;

      view.drawTexture(0,0,frameStars, WHITE);

      if(frameBeam == 0) {
          view.drawTexture(beamPos,view.getScreenHeight()/2 - view.getTextureHeight(BEAM_A)/2, BEAM_A, WHITE);
      }
      else
        view.drawTexture(beamPos,view.getScreenHeight()/2 - view.getTextureHeight(BEAM_B)/2, BEAM_B, WHITE);

      view.drawTexture(view.getScreenWidth()/2 - view.getTextureWidth(TITLE_)/2, titlePos, TITLE_, (Color){255,255,255,titleFade});

      if(beamPos + view.getTextureWidth(BEAM_A) > view.getScreenWidth()) {
          view.drawSprite(view.getScreenWidth() - (200.00*xScale), view.getScreenHeight()/2 - (200.00*yScale), BURST_SPRITESHEET, frameBurst, WHITE);
      }

      view.endFrame();
    }
    gamestatus = STATUS_MENU;
}

void Controller::doMenu() {
  int cursor_pos = 0;
  int frame = 0;
  int frameTicker = 0;
  int res = 0;
  while(gamestatus == STATUS_MENU) {

    control_info gamepad = view.getControlInfo();
    if(gamepad.enter_released) {
        switch (cursor_pos) {
            case 0:
                gamestatus = STATUS_LEVEL_SEL;
                break;
            case 1:
                gamestatus = STATUS_SETTINGS;
                break;
            case 2:
                gamestatus = STATUS_QUIT;
                break;
            case 3:
                gamestatus = STATUS_EDIT;
                break;
        }
    }

    if(gamepad.down_pressed) {
        cursor_pos += 1;
    }
    if(gamepad.up_pressed) {
        cursor_pos -= 1;
    }

    if(cursor_pos > 3) {
        cursor_pos = 0;
    }
    if(cursor_pos < 0) {
        cursor_pos = 3;
    }
    view.startFrame();

    frameTicker++;
    if(frameTicker > 5) {
        frameTicker = 0;
        frame += 1;
        if(frame > 11) frame = 0;
    }

    if(view.getWindowStatus()) break;

    view.drawTexture(0,0,frame, WHITE);

    view.drawTexture(view.getScreenWidth()/2 - view.getTextureWidth(TITLE_)/2, 0 + 50*yScale, TITLE_, WHITE);

    view.drawText("Play", view.getScreenWidth()/2, view.getScreenHeight()/2, 40*((xScale+yScale)/2), (cursor_pos == 0) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("Settings", view.getScreenWidth()/2, view.getScreenHeight()/2 + 40*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 1) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("Quit", view.getScreenWidth()/2, view.getScreenHeight()/2 + 80*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 2) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("Edit", view.getScreenWidth()/2, view.getScreenHeight()/2 + 120*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 3) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});

    view.endFrame();
  }
}

void Controller::doSettings(){
  int cursor_pos = 0;
  int frame = 0;
  int frameTicker = 0;
  int res = 0;

  while(gamestatus == STATUS_SETTINGS) {

    control_info gamepad = view.getControlInfo();
    if(gamepad.enter_released) {
        switch (cursor_pos) {
            case 0:
                view.initView(1280, 720, &xScale, &yScale);
                break;
            case 1: //temp change res. Coming soon: settingsMenu();
                view.initView(1366,768, &xScale, &yScale);
                break;
            case 2:
                view.initView(2560, 1440, &xScale, &yScale);
                break;
            case 3:
                view.initView(3840, 2160, &xScale, &yScale);
                break;
            case 4:
                view.setFullScreen(&xScale, &yScale);
                break;
            case 5:
                gamestatus = STATUS_MENU;
                break;
        }
    }

    if(gamepad.down_pressed) {
        cursor_pos += 1;
    }
    if(gamepad.up_pressed) {
        cursor_pos -= 1;
    }

    if(cursor_pos > 5) {
        cursor_pos = 0;
    }
    if(cursor_pos < 0) {
        cursor_pos = 5;
    }

    view.startFrame();

    frameTicker++;
    if(frameTicker > 5) {
        frameTicker = 0;
        frame += 1;
        if(frame > 11) frame = 0;
    }

    if(view.getWindowStatus()) break;

    view.drawTexture(0,0,frame, WHITE);

    view.drawText("Settings", view.getScreenWidth()/2, view.getScreenHeight()/2 - 100*((xScale+yScale)/2), 100*((xScale+yScale)/2), (Color) {255,255,255,255});

    view.drawText("1280x720", view.getScreenWidth()/2, view.getScreenHeight()/2 + 40*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 0) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("1920x1080", view.getScreenWidth()/2, view.getScreenHeight()/2 + 80*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 1) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("2560x1440", view.getScreenWidth()/2, view.getScreenHeight()/2 + 120*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 2) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("3840x2160", view.getScreenWidth()/2, view.getScreenHeight()/2 + 160*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 3) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("FullScreen", view.getScreenWidth()/2, view.getScreenHeight()/2 + 200*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 4) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("Back", view.getScreenWidth()/2, view.getScreenHeight()/2 + 240*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 5) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});



    view.endFrame();
  }
}

void Controller::doGame() {
  pause = false;

  player.update = &player_update;
  player.setX(100);
  player.setY(100);

  int bulletCounter = 0;
  bool bulletReset = false;

  Entity burner;
  burner.timer = 0;
  burner.frame = 0;
  burner.textureName = BURNER_SPRITE_SHEET;
  burner.update = &burner_update;

  player.rect.width = 132.0*xScale;
  player.rect.height = 161.0*yScale;

  screenWidth_model = view.getScreenWidth();
  screenHeight_model = view.getScreenHeight();

  for(int i = 0; i < 15; i++) {
    //spawn_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), xScale, yScale, &enemies);
  }

  //testLevel();
  //generate_Level("assets/levels/level_test.txt");

  while(gamestatus == STATUS_PLAYING) {
    if(view.getWindowStatus()) break;


    control_info gamepad = view.getControlInfo();

    if(!pause) {
      if(gamepad.up_held) {
        player.setYvel(-5.0*yScale);
      }
      else if(gamepad.down_held) {
        player.setYvel(5.0*yScale);
      }
      else {
        player.setYvel(0);
      }

      if(gamepad.left_held) {
        player.setXvel(-5.0*xScale);
      }
      else if(gamepad.right_held) {
        player.setXvel(5.0*xScale);
      }
      else {
        player.setXvel(0);
      }

      if(gamepad.enter_held) {
        bulletCounter++;
        if(bulletCounter > 5 && !bulletReset){
          spawn_bullet(player.rect.x + (player.rect.width / 2) - 4*xScale, player.rect.y, 0, xScale, yScale, &bullets);
          bulletCounter = 0;
        }
      }
      else {
        bulletCounter = 0;
      }
      if(gamepad.pause_pressed) {
        pause = true;
        pause_menu_pos = 0;
      }
    }

    if(bullets.size() >= 3) {
      bulletReset = true;
      bulletCounter = 0;
    }
    if(bullets.size() == 0) {
      bulletReset = false;
    }

    if(bulletReset) {
      bulletCounter = 0;
    }

    if(!pause) {
      entityLoop();
    }

    burner.rect.x = player.rect.x + player.rect.width/2 - 22*xScale/2;

    burner.rect.y = player.rect.y + 130*yScale;

    view.startFrame();
    view.drawTexture(0,0,0, WHITE);

    //DrawRectangleRec(player.getRect(), GREEN);
    view.drawSprite(burner.getX(), burner.getY(), BURNER_SPRITE_SHEET, burner.frame, WHITE);
    view.drawSprite(player.getX(), player.getY(), PLAYER_SPRITESHEET, player.getFrame(), WHITE);

    self = &burner;
    if(burner.update != NULL) burner.update();

    //enemies draw loop
    for(int i = 0; i < enemies.size(); i++) {
      //DrawRectangleRec(enemies[i].getRect(), GREEN);
      view.drawSprite(enemies[i].getX(), enemies[i].getY(), enemies[i].textureName, enemies[i].getFrame(), WHITE);
    }

    for(int i = 0; i < bullets.size(); i++) {
      //DrawRectangleRec(bullets[i].getRect(), GREEN);
      view.drawSprite(bullets[i].getX(), bullets[i].getY(), bullets[i].textureName, bullets[i].getFrame(), WHITE);
    }

    for(int i = 0; i < enemy_bullets.size(); i++) {
      //DrawRectangleRec(bullets[i].getRect(), GREEN);
      view.drawSprite(enemy_bullets[i].getX(), enemy_bullets[i].getY(), enemy_bullets[i].textureName, enemy_bullets[i].getFrame(), WHITE);
    }

    for(int i = 0; i < explosions.size(); i++) {
      //DrawRectangleRec(bullets[i].getRect(), GREEN);
      view.drawSprite(explosions[i].getX(), explosions[i].getY(), explosions[i].textureName, explosions[i].getFrame(), WHITE);
    }

    std::string points_string = "Points: ";
    points_string += std::to_string(points);

    view.drawText(points_string, 20, 20, 40, WHITE);

    if(pause) {
      doPauseMenu(gamepad);
    }

    view.endFrame();
  }

  while(!enemies.empty()) {
    enemies.pop_back();
  }

  while(!bullets.empty()) {
    bullets.pop_back();
  }

  while(!explosions.empty()) {
    explosions.pop_back();
  }
}

void Controller::testLevel() {
  for(int i = 0; i < 10; i += 2) {
    //spawn_enemy(view.getScreenWidth() - 400*xScale, -200*yScale - view.getScreenHeight()*i, xScale, yScale, &enemies);
    spawn_enemy(view.getScreenWidth() - 400*xScale, -200*yScale - (view.getScreenHeight()-200*yScale) - view.getScreenHeight()*i, xScale, yScale, 0, &enemies);
  }
  for(int i = 1; i < 10; i += 2) {
    //spawn_enemy(view.getScreenWidth() - 400*xScale, -200*yScale - view.getScreenHeight()*i, xScale, yScale, &enemies);
    spawn_enemy(400*xScale - enemies[0].rect.width, -200*yScale - (view.getScreenHeight()-200*yScale) - view.getScreenHeight()*i, xScale, yScale, 0, &enemies);
  }

  for(int i = 0; i < 12; i += 3) {
    spawn_enemy(view.getScreenWidth()/2, -200*yScale - (view.getScreenHeight()-200*yScale) - view.getScreenHeight()*i, xScale, yScale, 1, &enemies);
  }
  for(int i = 0; i < 12; i += 3) {
    spawn_enemy(view.getScreenWidth()/2, -200*yScale - (view.getScreenHeight()-200*yScale) - view.getScreenHeight()*i, xScale, yScale, 2, &enemies);
  }
}

void Controller::generate_Level(std::string level){
  std::ifstream myfile (level);
  std::string line;
  if (myfile.is_open()){
    while(getline(myfile,line)){
      std::size_t pos = line.find(",");
      int data [3];
      data[0] = data[1] = data[2] = 0;
      int count = 0;
      while(pos != std::string::npos) {
        data[count] = std::stoi(line.substr(0,pos));
        std::cout << line.substr(0,pos).c_str();
        std::cout << ",";
        line.erase(0,pos + 1);
        count += 1;
        pos = line.find(",");
      }
      std::cout << std::endl;
      spawn_enemy((float)data[0]*xScale, (float)data[1]*yScale, xScale, yScale, data[2], &enemies);
    }
    myfile.close();
  }
}

void Controller::doPauseMenu(control_info gamepad) {
  //check buttons
  if(gamepad.enter_released) {
      switch (pause_menu_pos) {
          case 0:
              pause = false;
              break;
          case 1: //temp change res. Coming soon: settingsMenu();
              pause = false;
              gamestatus = STATUS_MENU;
              break;
      }
  }

  if(gamepad.up_pressed) {
    pause_menu_pos--;
  }
  if(gamepad.down_pressed) {
    pause_menu_pos++;
  }

  if(pause_menu_pos < 0) {
    pause_menu_pos = 1;
  }
  if(pause_menu_pos > 1) {
    pause_menu_pos = 0;
  }

  view.drawRectBorders(view.getScreenWidth()/2 - view.getScreenWidth()/4, view.getScreenHeight()/2-view.getScreenHeight()/4, view.getScreenWidth()/2, view.getScreenHeight()/2, (Color) {58,50,168,255}, (Color) {0,0,0,255});

  int textWidth = MeasureText("Finding Unknown", 60*((xScale+yScale)/2));
  view.drawText("Finding Unknown", view.getScreenWidth()/2 - textWidth/2, view.getScreenHeight()/4 + 20*yScale, 60*((xScale+yScale)/2), (Color) {255,0,0,255});

  textWidth = MeasureText("Return", 40*((xScale+yScale)/2));
  view.drawText("Return", view.getScreenWidth()/2 - textWidth/2, view.getScreenHeight()/2, 40*((xScale+yScale)/2), (pause_menu_pos == 0) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
  textWidth = MeasureText("Quit", 40*((xScale+yScale)/2));
  view.drawText("Quit", view.getScreenWidth()/2  - textWidth/2, view.getScreenHeight()/2 + 40*((xScale+yScale)/2), 40*((xScale+yScale)/2), (pause_menu_pos == 1) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
}

void Controller::doEditMenu(control_info gamepad, int level_start, std::string path) {
  //check buttons
  if(gamepad.enter_released) {
      switch (pause_menu_pos) {
          case 0:
              levelToTxt(enemies, level_start, path);
              pause = false;
              break;
          case 1:
              pause = false;
              break;
          case 2: //temp change res. Coming soon: settingsMenu();
              pause = false;
              gamestatus = STATUS_MENU;
              break;
      }
  }

  if(gamepad.up_pressed) {
    pause_menu_pos--;
  }
  if(gamepad.down_pressed) {
    pause_menu_pos++;
  }

  if(pause_menu_pos < 0) {
    pause_menu_pos = 2;
  }
  if(pause_menu_pos > 2) {
    pause_menu_pos = 0;
  }

  view.drawRectBorders(view.getScreenWidth()/2 - view.getScreenWidth()/4, view.getScreenHeight()/2-view.getScreenHeight()/4, view.getScreenWidth()/2, view.getScreenHeight()/2, (Color) {58,50,168,255}, (Color) {0,0,0,255});

  int textWidth = MeasureText("Finding Unknown", 60*((xScale+yScale)/2));
  view.drawText("Finding Unknown", view.getScreenWidth()/2 - textWidth/2, view.getScreenHeight()/4 + 20*yScale, 60*((xScale+yScale)/2), (Color) {255,0,0,255});

  textWidth = MeasureText("Save", 40*((xScale+yScale)/2));
  view.drawText("Save", view.getScreenWidth()/2 - textWidth/2, view.getScreenHeight()/2, 40*((xScale+yScale)/2), (pause_menu_pos == 0) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
  textWidth = MeasureText("Return", 40*((xScale+yScale)/2));
  view.drawText("Return", view.getScreenWidth()/2 - textWidth/2, view.getScreenHeight()/2 + 40*((xScale+yScale)/2), 40*((xScale+yScale)/2), (pause_menu_pos == 1) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
  textWidth = MeasureText("Quit", 40*((xScale+yScale)/2));
  view.drawText("Quit", view.getScreenWidth()/2  - textWidth/2, view.getScreenHeight()/2 + 80*((xScale+yScale)/2), 40*((xScale+yScale)/2), (pause_menu_pos == 2) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
}

void Controller::createLevel() {
  int level_start = view.getScreenHeight(); //the start of the level. things are reletive to this on output.
  int level_end = level_start - view.getScreenHeight() * 20;
  int cursor_x = 0;
  int cursor_y = 0;
  int cur_type = 0;

  textBox name_box(view.getScreenWidth() - 300*xScale, 40*xScale, 290*xScale, 35*xScale, 30);

  pause = false;

  while(gamestatus == STATUS_EDIT) {
    if(view.getWindowStatus()) break;

    level_end = level_start - view.getScreenHeight() * 20; //always reletive to level_start;

    name_box.update();

    control_info gamepad = view.getControlInfo();

    if(!name_box.mouseOnText && !pause) {
      if(gamepad.up_held) {
        cursor_y -= 5;
      }
      if(cursor_y <= 0) {
        cursor_y += 5;
        if(level_end < 0) {
          level_start += 5;
          for(int i = 0; i < enemies.size(); i++) {
            //DrawRectangleRec(enemies[i].getRect(), GREEN);
            enemies[i].rect.y += 5;
          }
        }
      }

      if(gamepad.down_held) {
        cursor_y += 5;
      }
      if(cursor_y >= view.getScreenHeight()) {
        cursor_y -= 5;
        if(level_start > view.getScreenHeight()) {
          level_start -= 5;

          for(int i = 0; i < enemies.size(); i++) {
            //DrawRectangleRec(enemies[i].getRect(), GREEN);
            enemies[i].rect.y -= 5;
          }
        }
      }
      if(gamepad.left_held) {
        cursor_x -= 5;
      }
      if(cursor_x <= 0) {
        cursor_x += 5;
      }
      else if(gamepad.right_held) {
        cursor_x += 5;
      }
      if(cursor_x >= view.getScreenWidth()) {
        cursor_x -= 5;
      }

      if(gamepad.enter_released) {
        spawn_enemy(cursor_x, cursor_y, xScale, yScale, cur_type, &enemies);
      }

      if(gamepad.pause_pressed) {
        pause = true;
        pause_menu_pos = 0;
      }

      if(gamepad.space_released) {
        cur_type += 1;
      }
    }

    if(cur_type > 3) {
      cur_type = 0;
    }

    view.startFrame();

    for(int i = level_start; i > level_end; i -= view.getScreenHeight()) {
      view.drawRectBorders(0,i,view.getScreenWidth(), 10, WHITE, WHITE);
    }

    for(int i = 0; i < enemies.size(); i++) {
      //DrawRectangleRec(enemies[i].getRect(), GREEN);
      view.drawSprite(enemies[i].getX(), enemies[i].getY(), enemies[i].textureName, enemies[i].getFrame(), WHITE);
    }

    switch (cur_type) {
      case 0:
        view.drawSprite(cursor_x, cursor_y, BASIC_ENEMY_SPRITE_SHEET, 0, WHITE);
        view.drawText("Basic", cursor_x,cursor_y - 50*yScale, 50*yScale, WHITE);
        break;
      case 1:
        view.drawSprite(cursor_x, cursor_y, ADV_ENEMY_SPRITE_SHEET, 0, WHITE);
        view.drawText("Advanced", cursor_x,cursor_y - 50*yScale, 50*yScale, WHITE);
        break;
      case 2:
        view.drawSprite(cursor_x, cursor_y, HARD_ENEMY_SPRITE_SHEET, 0, WHITE);
        view.drawText("Tracker", cursor_x,cursor_y - 50*yScale, 50*yScale, WHITE);
        break;
      case 3:
        view.drawSprite(cursor_x, cursor_y, BASIC_ENEMY_SPRITE_SHEET, 0, WHITE);
        view.drawText("Bomber", cursor_x,cursor_y - 50*yScale, 50*yScale, WHITE);
        break;
    }

    view.drawText("level start", 0,level_start - 50*yScale, 50*yScale, WHITE);
    view.drawText("level end", 0,level_end + 20, 50*yScale, WHITE);

    name_box.draw();


    view.drawText("edit level", 20, 20, 40, WHITE);

    if(pause) {
      std::string path = "assets/levels/";
      path += name_box.message;
      path += ".txt";
      doEditMenu(gamepad, level_start, path);
    }

    view.endFrame();
  }
}

void Controller::levelToTxt(std::vector<Entity> ent_list, int level_start, std::string file_path) {
  std::ofstream file;
  file.open (file_path.c_str());
  for(int i = 0; i < ent_list.size(); i++) {
    int x = (enemies[i].rect.x/xScale);
    int y = (0 - (level_start -  enemies[i].rect.y)/yScale);

    file << x << "," << y << "," << enemies[i].className << "," << std::endl;
  }

  file.close();
}

void Controller::levelSelect() {
  std::vector<std::string> level_list; //to hold the list of level names.

  int cursor_pos = 0; //cursor position.
  int y_offset = 20; //for scrolling list. 

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir ("assets/levels/")) != NULL) {
    //read filenames and add files > 2 chars to level_list.
    while ((ent = readdir (dir)) != NULL) {
      std::string filename = ent->d_name;
      if(filename.length() > 2) {
        level_list.push_back(ent->d_name);
      }
    }
    closedir (dir);
  }

  int list_size = static_cast<int>(level_list.size());

  while(gamestatus == STATUS_LEVEL_SEL) {
    if(view.getWindowStatus()) break;


    control_info gamepad = view.getControlInfo();

    if(!pause) {
      if(gamepad.up_released) {
        player.setYvel(-5.0*yScale);
        cursor_pos -= 1;
      }
      else if(gamepad.down_released) {
        cursor_pos += 1;
      }
      if(gamepad.enter_released) { //load level and start playing.
        std::string finalPath = "assets/levels/";
        finalPath += level_list[cursor_pos];
        generate_Level(finalPath);
        gamestatus = STATUS_PLAYING;
      }
    }
    if(cursor_pos > list_size - 1) {
      cursor_pos = 0;
    }
    if(cursor_pos < 0) {
      cursor_pos = list_size - 1;
    }

    //correct for scrolling to low.
    while(y_offset + (cursor_pos*50*yScale) > view.getScreenHeight() - 50*yScale) {
      y_offset -= 50*yScale;
    }

    //correct for scrolling to high.
    while(y_offset + (cursor_pos*50*yScale) < 20) {
      y_offset += 50*yScale;
    }

    view.startFrame();

    for(int i = 0; i < list_size; i++) {
      view.drawText(level_list[i], 20, y_offset + i*50*yScale, 50*yScale, (cursor_pos == i) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    }

    //view.drawText(points_string, 20, 20, 40, WHITE);

    view.endFrame();
  }
}
