#include "Controller.h"

Controller::Controller() {
  gamestatus = STATUS_START;
  xScale = 1;
  yScale = 1;
  points = 0;

  view.initView(1280,720, &xScale, &yScale);

  gameLoop();
}

void Controller::entityLoop() {
  self = &player;
  player.update();

  for(int i = 0; i < (int) enemies.size(); i++) {
    self = &enemies[i];
    enemies[i].update();

    for(int x = 0; x < bullets.size(); x++) {
      other = &bullets[i];
      if(CheckCollisionRecs(enemies[i].rect, bullets[x].rect)) {
        enemies[i].hit();
      }
    }

    if(enemies[i].status == ENTITY_REMOVE) {
      enemies.erase(enemies.begin() + i);
      spawn_test_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), &enemies);
    }
    if(enemies[i].status == ENTITY_DESTROY) {
      points += enemies[i].points;
      enemies.erase(enemies.begin() + i);
      spawn_test_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), &enemies);
    }
  }

  for(int i = 0; i < (int) bullets.size(); i++) {
    self = &bullets[i];
    bullets[i].update();

    if(bullets[i].status == ENTITY_REMOVE) {
      bullets.erase(bullets.begin() + i);
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
                gamestatus = STATUS_PLAYING;
                break;
            case 1:
                gamestatus = STATUS_SETTINGS;
                break;
            case 2:
                gamestatus = STATUS_QUIT;
                break;
        }
    }

    if(gamepad.down_pressed) {
        cursor_pos += 1;
    }
    if(gamepad.up_pressed) {
        cursor_pos -= 1;
    }

    if(cursor_pos > 2) {
        cursor_pos = 0;
    }
    if(cursor_pos < 0) {
        cursor_pos = 2;
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

    if(cursor_pos > 4) {
        cursor_pos = 0;
    }
    if(cursor_pos < 0) {
        cursor_pos = 4;
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
    view.drawText("Back", view.getScreenWidth()/2, view.getScreenHeight()/2 + 200*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 4) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});



    view.endFrame();
  }
}

void Controller::doGame() {
  player.update = &player_update;
  player.setX(100);
  player.setY(100);

  player.rect.width = 150.0*xScale;
  player.rect.height = 200*yScale;

  screenWidth_model = view.getScreenWidth();
  screenHeight_model = view.getScreenHeight();

  for(int i = 0; i < 5; i++) {
    spawn_test_enemy(rand() % (int)view.getScreenWidth(), 0 - rand() % (int)view.getScreenHeight(), &enemies);
  }

  while(gamestatus == STATUS_PLAYING) {
    if(view.getWindowStatus()) break;


    control_info gamepad = view.getControlInfo();

    if(gamepad.up_held) {
      player.setYvel(-5.0);
    }
    else if(gamepad.down_held) {
      player.setYvel(5.0);
    }
    else {
      player.setYvel(0);
    }

    if(gamepad.left_held) {
      player.setXvel(-5.0);
    }
    else if(gamepad.right_held) {
      player.setXvel(5.0);
    }
    else {
      player.setXvel(0);
    }

    if(gamepad.enter_released) {
      spawn_bullet(player.rect.x + (player.rect.width / 2) - 4*xScale, player.rect.y, 0, &bullets);
    }

    entityLoop();

    view.startFrame();
    view.drawTexture(0,0,0, WHITE);

    DrawRectangleRec(player.getRect(), GREEN);
    view.drawSprite(player.getX(), player.getY(), PLAYER_SPRITESHEET, player.getFrame(), WHITE);

    //enemies draw loop
    for(int i = 0; i < enemies.size(); i++) {
      DrawRectangleRec(enemies[i].getRect(), GREEN);
    }

    for(int i = 0; i < bullets.size(); i++) {
      //DrawRectangleRec(bullets[i].getRect(), GREEN);
      view.drawSprite(bullets[i].getX(), bullets[i].getY(), bullets[i].textureName, bullets[i].getFrame(), WHITE);
    }

    std::string points_string = "Points: ";
    points_string += std::to_string(points);

    view.drawText(points_string, 20, 20, 40, WHITE);

    view.endFrame();
  }
}
