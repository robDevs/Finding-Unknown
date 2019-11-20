#include "Controller.h"

Controller::Controller() {
  gamestatus = STATUS_START;
  xScale = 1;
  yScale = 1;
  points = 0;

  srand (time(NULL));

  view.initView(1280,720, &xScale, &yScale);

  gameLoop();
}

void Controller::entityLoop() {
  self = &player;
  if(player.update != NULL) player.update();

  for(int i = 0; i < (int) enemies.size(); i++) {
    self = &enemies[i];
    if(enemies[i].update != NULL) enemies[i].update();

    for(int x = 0; x < bullets.size(); x++) {
      other = &bullets[x];
      if(CheckCollisionRecs(enemies[i].rect, bullets[x].rect)) {
        if(enemies[i].hit != NULL) enemies[i].hit();
      }
    }

    if(enemies[i].status == ENTITY_REMOVE) {
      enemies.erase(enemies.begin() + i);
      spawn_test_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), xScale, yScale, &enemies);
    }
    if(enemies[i].status == ENTITY_DESTROY) {
      spawn_explosion(enemies[i].rect.x + enemies[i].rect.width/2 - 200*xScale, enemies[i].rect.y + enemies[i].rect.width/2 - 200*yScale, &explosions);
      points += enemies[i].points;
      enemies.erase(enemies.begin() + i);
      spawn_test_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), xScale, yScale,  &enemies);
    }
  }

  for(int i = 0; i < (int) bullets.size(); i++) {
    self = &bullets[i];
    if(bullets[i].update != NULL) bullets[i].update();

    if(bullets[i].status == ENTITY_REMOVE) {
      bullets.erase(bullets.begin() + i);
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
  player.update = &player_update;
  player.setX(100);
  player.setY(100);

  int bulletCounter = 0;

  Entity burner;
  burner.timer = 0;
  burner.frame = 0;
  burner.textureName = BURNER_SPRITE_SHEET;
  burner.update = &burner_update;

  player.rect.width = 150.0*xScale;
  player.rect.height = 200*yScale;

  screenWidth_model = view.getScreenWidth();
  screenHeight_model = view.getScreenHeight();

  for(int i = 0; i < 15; i++) {
    spawn_test_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), xScale, yScale, &enemies);
  }

  while(gamestatus == STATUS_PLAYING) {
    if(view.getWindowStatus()) break;


    control_info gamepad = view.getControlInfo();

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
      if(bulletCounter > 5) {
        spawn_bullet(player.rect.x + (player.rect.width / 2) - 4*xScale, player.rect.y, 0, xScale, yScale, &bullets);
        bulletCounter = 0;
      }
    }
    else {
      bulletCounter = 0;
    }

    entityLoop();

    burner.rect.x = player.rect.x + player.rect.width/2 - 22*xScale/2;

    burner.rect.y = player.rect.y + 152*yScale;

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

    for(int i = 0; i < explosions.size(); i++) {
      //DrawRectangleRec(bullets[i].getRect(), GREEN);
      view.drawSprite(explosions[i].getX(), explosions[i].getY(), explosions[i].textureName, explosions[i].getFrame(), WHITE);
    }

    std::string points_string = "Points: ";
    points_string += std::to_string(points);

    view.drawText(points_string, 20, 20, 40, WHITE);

    view.endFrame();
  }
}
