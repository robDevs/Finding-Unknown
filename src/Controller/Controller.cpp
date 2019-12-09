#include "Controller.h"

Controller::Controller() {
  gamestatus = STATUS_START;
  xScale = 1;
  yScale = 1;
  points = 0;
  flashing = 0;
  respawn_timer = 181;
  pause = false;
  pause_menu_pos = 0;

  current_level = "";

  view.initView(1280,720, &xScale, &yScale);

  view.playSound(5);

  gameLoop();
}

void Controller::entityLoop() {
  player_pointer = &player;
  if(player.update != NULL) player.update();

  if(player.status == ENTITY_DESTROY){
      spawn_explosion(player.rect.x + player.rect.width/2 - 200*xScale, player.rect.y + player.rect.width/2 - 200*yScale, 0, &explosions);
      player.hit = NULL;
      respawn_timer = 0;
      flashing = 0;
      player.health = 1;
      player.status = ENTITY_KEEP;

      if(extraLives == 0) {
          gameOver = true;
      }
      else {
          extraLives -= 1;
      }
  }
  if(respawn_timer < 181){
      player.status = ENTITY_KEEP;
      respawn_timer++;
      if(respawn_timer == 180){
          player.hit = &player_hit;
      }
  }

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
        view.playSound(SFX_HIT);
      }
    }

    if(CheckCollisionRecs(player.rect, enemies[i].rect) && respawn_timer > 180) {
        other = &player;
        if(enemies[i].hit != NULL) enemies[i].hit();
    }

    if(enemies[i].status == ENTITY_REMOVE) {
      enemies.erase(enemies.begin() + i);
      //spawn_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), xScale, yScale, &enemies);
    }
    if(enemies[i].status == ENTITY_DESTROY) {
      spawn_explosion(enemies[i].rect.x + enemies[i].rect.width/2 - 200*xScale, enemies[i].rect.y + enemies[i].rect.width/2 - 200*yScale, 0, &explosions);
      view.playSound(SFX_EXP);
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

    if(CheckCollisionRecs(player.rect, enemy_bullets[i].rect)  && respawn_timer > 180) {
        other = &player;
        if(enemy_bullets[i].hit != NULL) enemy_bullets[i].hit();
    }

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
      gamestatus = STATUS_LEVEL_SEL;
      levelSelect(false);
      doGame();
    }
    if(gamestatus == STATUS_SETTINGS) {
      doSettings();
    }
    if(gamestatus == STATUS_EDIT) {
      gamestatus = STATUS_LEVEL_SEL;
      levelSelect(true);
      createLevel();
    }
  }
  view.freeTextures();
  view.freeSounds();
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

    view.drawText("Play", (view.getScreenWidth()/2) - (MeasureText("Play", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2, 40*((xScale+yScale)/2), (cursor_pos == 0) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("Settings", (view.getScreenWidth()/2) - (MeasureText("Settings", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 40*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 1) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("Quit", (view.getScreenWidth()/2) - (MeasureText("Quit", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 80*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 2) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("Edit", (view.getScreenWidth()/2) - (MeasureText("Edit", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 120*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 3) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});

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

    view.drawText("Settings", view.getScreenWidth()/2 - (MeasureText("Settings", 100*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 - 100*((xScale+yScale)/2), 100*((xScale+yScale)/2), (Color) {255,255,255,255});

    view.drawText("1280x720", view.getScreenWidth()/2 - (MeasureText("1280x720", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 40*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 0) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("1920x1080", view.getScreenWidth()/2 - (MeasureText("1920x1080", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 80*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 1) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("2560x1440", view.getScreenWidth()/2 - (MeasureText("2560x1440", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 120*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 2) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("3840x2160", view.getScreenWidth()/2 - (MeasureText("3840x2160", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 160*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 3) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("FullScreen", view.getScreenWidth()/2 - (MeasureText("FullScreen", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 200*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 4) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
    view.drawText("Back", view.getScreenWidth()/2 - (MeasureText("Back", 40*(xScale+yScale)/2) / 2), view.getScreenHeight()/2 + 240*((xScale+yScale)/2), 40*((xScale+yScale)/2), (cursor_pos == 5) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});



    view.endFrame();
  }
}

void Controller::doGame() {
  pause = false;

  player.update = &player_update;
  player.hit = &player_hit;
  player.lives = 3;
  player.setX(100);
  player.setY(100);
  player.health = 1;
  player.className = PLAYER_PLAYER;

  extraLives = 3;
  gameOver = false;
  levelComplete = false;

  points = 0;

  int endTimer = 0;

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

  int bg_pos = 0;
  int bg_pos1 = -1440;

  int bg_frame = 0;
  int bg_frame_time = 0;

  for(int i = 0; i < 15; i++) {
    //spawn_enemy(rand() % (int)view.getScreenWidth(), -50 - rand() % (int)view.getScreenHeight(), xScale, yScale, &enemies);
  }

  //testLevel();
  //generate_Level("assets/levels/level_test.txt");

  while(gamestatus == STATUS_PLAYING) {
    if(view.getWindowStatus()) break;

    bg_pos += 2;
    bg_pos1 += 2;

    bg_frame_time++;
    if(bg_frame_time >= 5) {
      bg_frame += 1;
      bg_frame_time = 0;
    }
    if(bg_frame > 5) {
      bg_frame = 0;
    }

    if(bg_pos >= view.getScreenHeight()) {
      bg_pos = -1440;
    }
    if(bg_pos1 >= view.getScreenHeight()) {
      bg_pos1 = -1440;
    }

    control_info gamepad = view.getControlInfo();
    if(respawn_timer<55){
        player.setXvel(0);
        player.setYvel(0);
    }
    if(!pause && respawn_timer > 55 && !gameOver && !levelComplete) {
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
          view.playSound(SFX_SHOOT);
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

    if(enemies.empty() && explosions.empty()) {
        levelComplete = true;
    }

    burner.rect.x = player.rect.x + player.rect.width/2 - 22*xScale/2;

    burner.rect.y = player.rect.y + 130*yScale;

    if(levelComplete || gameOver) {
        endTimer++;
        player.xVel = 0;
        player.yVel = 0;
    }
    if(endTimer > 60*5) {
        gamestatus = STATUS_START;
    }

    view.startFrame();

    switch (bg_frame) {
      case 0:
        view.drawTexture(0,bg_pos, BG_SCROLL_01, WHITE);
        view.drawTexture(0,bg_pos1, BG_SCROLL_01, WHITE);
        break;
      case 1:
        view.drawTexture(0,bg_pos, BG_SCROLL_02, WHITE);
        view.drawTexture(0,bg_pos1, BG_SCROLL_02, WHITE);
        break;
      case 2:
        view.drawTexture(0,bg_pos, BG_SCROLL_03, WHITE);
        view.drawTexture(0,bg_pos1, BG_SCROLL_03, WHITE);
        break;
      case 3:
        view.drawTexture(0,bg_pos, BG_SCROLL_03, WHITE);
        view.drawTexture(0,bg_pos1, BG_SCROLL_03, WHITE);
        break;
      case 4:
        view.drawTexture(0,bg_pos, BG_SCROLL_04, WHITE);
        view.drawTexture(0,bg_pos1, BG_SCROLL_04, WHITE);
        break;
      case 5:
        view.drawTexture(0,bg_pos, BG_SCROLL_05, WHITE);
        view.drawTexture(0,bg_pos1, BG_SCROLL_05, WHITE);
        break;
      case 6:
        view.drawTexture(0,bg_pos, BG_SCROLL_06, WHITE);
        view.drawTexture(0,bg_pos1, BG_SCROLL_06, WHITE);
        break;
    }

    //DrawRectangleRec(player.getRect(), GREEN);
    if(respawn_timer % 10 == 0) flashing++;
    if(respawn_timer > 55 && flashing % 2 == 0){
        view.drawSprite(burner.getX(), burner.getY(), BURNER_SPRITE_SHEET, burner.frame, WHITE);
        view.drawSprite(player.getX(), player.getY(), PLAYER_SPRITESHEET, player.getFrame(), WHITE);
    }

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

    view.drawText(points_string, 20, 20*yScale, 40, WHITE);

    std::string lives_string = "Extra Ships: ";
    lives_string += std::to_string(extraLives);
    view.drawText(lives_string, view.getScreenWidth() - MeasureText(lives_string.c_str(), 40*yScale) - 20, 20*yScale, 40, WHITE);

    if(pause) {
      doPauseMenu(gamepad);
    }

    if(levelComplete) {
        view.drawText("Level Complete!", (view.getScreenWidth() / 2) - (MeasureText("Level Complete!", 70*yScale) / 2), 20*yScale, 70*yScale, BLUE);
    }

    if(gameOver) {
        view.drawText("Game Over!", (view.getScreenWidth() / 2) - (MeasureText("Game Over!", 70*yScale) / 2), 20*yScale, 70*yScale, RED);
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

  if(gameOver) gamestatus = STATUS_START;
  if(levelComplete) gamestatus = STATUS_PLAYING;
}

void Controller::level_one() {
  spawn_enemy((float)810*xScale, (float)-510*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)305*xScale, (float)-510*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)310*xScale, (float)-1000*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)820*xScale, (float)-1000*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)545*xScale, (float)-1250*yScale, xScale, yScale, 1, &enemies);
  spawn_enemy((float)250*xScale, (float)-1855*yScale, xScale, yScale, 2, &enemies);
  spawn_enemy((float)540*xScale, (float)-3040*yScale, xScale, yScale, 4, &enemies);
  spawn_enemy((float)330*xScale, (float)-2400*yScale, xScale, yScale, 3, &enemies);
  spawn_enemy((float)810*xScale, (float)-2390*yScale, xScale, yScale, 3, &enemies);
  spawn_enemy((float)590*xScale, (float)-2640*yScale, xScale, yScale, 2, &enemies);
}
void Controller::level_two() {
  spawn_enemy((float)140*xScale, (float)-270*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)315*xScale, (float)-415*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)465*xScale, (float)-555*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)625*xScale, (float)-710*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)780*xScale, (float)-870*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)515*xScale, (float)-1150*yScale, xScale, yScale, 1, &enemies);
  spawn_enemy((float)750*xScale, (float)-1490*yScale, xScale, yScale, 3, &enemies);
  spawn_enemy((float)535*xScale, (float)-1635*yScale, xScale, yScale, 3, &enemies);
  spawn_enemy((float)310*xScale, (float)-1805*yScale, xScale, yScale, 3, &enemies);
  spawn_enemy((float)140*xScale, (float)-1975*yScale, xScale, yScale, 3, &enemies);
  spawn_enemy((float)305*xScale, (float)-2315*yScale, xScale, yScale, 2, &enemies);
  spawn_enemy((float)810*xScale, (float)-2565*yScale, xScale, yScale, 2, &enemies);
  spawn_enemy((float)225*xScale, (float)-2870*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)425*xScale, (float)-3025*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)625*xScale, (float)-3195*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)790*xScale, (float)-3335*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)940*xScale, (float)-3485*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)720*xScale, (float)-3785*yScale, xScale, yScale, 1, &enemies);
  spawn_enemy((float)180*xScale, (float)-4080*yScale, xScale, yScale, 3, &enemies);
  spawn_enemy((float)385*xScale, (float)-4315*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)630*xScale, (float)-4500*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)385*xScale, (float)-4705*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)620*xScale, (float)-4900*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)385*xScale, (float)-5085*yScale, xScale, yScale, 0, &enemies);
  spawn_enemy((float)510*xScale, (float)-5490*yScale, xScale, yScale, 1, &enemies);
  spawn_enemy((float)515*xScale, (float)-6100*yScale, xScale, yScale, 4, &enemies);
  spawn_enemy((float)870*xScale, (float)-6730*yScale, xScale, yScale, 2, &enemies);
  spawn_enemy((float)205*xScale, (float)-6490*yScale, xScale, yScale, 2, &enemies);
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
              levelToTxt(enemies, level_start, path, false);
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
  if(gamepad.space_released) {
      switch (pause_menu_pos) {
          case 0:
              levelToTxt(enemies, level_start, "assets/level_code/output.txt", true);
              pause = false;
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
  const char *temp = current_level.c_str();
  name_box.setMessage(temp);

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
      if(gamepad.back_released) {
        for(int i = 0; i < enemies.size(); i++) {
          if(CheckCollisionPointRec((Vector2){cursor_x, cursor_y}, enemies[i].rect)) {
            enemies.erase(enemies.begin() + i);
          }
        }
      }
    }

    if(cur_type > 4) {
      cur_type = 0;
    }

    view.startFrame();

    for(int i = level_start; i > level_end; i -= view.getScreenHeight()) {
      view.drawRectBorders(0,i,view.getScreenWidth(), 10, WHITE, WHITE);
    }

    for(int i = 0; i < enemies.size(); i++) {
      if(CheckCollisionPointRec((Vector2){cursor_x, cursor_y}, enemies[i].rect)) {
        view.drawRectBorders(enemies[i].rect.x, enemies[i].rect.y,enemies[i].rect.width,enemies[i].rect.height, RED, BLACK);
      }
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
      case 4:
        view.drawSprite(cursor_x, cursor_y, FIGHTER_SPRITESHEET, 1, WHITE);
        view.drawText("Fighter", cursor_x,cursor_y - 50*yScale, 50*yScale, WHITE);
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

  while(!enemies.empty()) {
    enemies.pop_back();
  }
}

void Controller::levelToTxt(std::vector<Entity> ent_list, int level_start, std::string file_path, bool code) {
  std::ofstream file;
  for(int i = 0; i < file_path.length(); i++) {
    if(file_path[i] == ' ') {
      file_path[i] = '_';
    }
  }
  file.open (file_path.c_str());
  for(int i = 0; i < ent_list.size(); i++) {
    int x = (enemies[i].rect.x/xScale);
    int y = (0 - (level_start -  enemies[i].rect.y)/yScale);

    if(!code){
        file << x << "," << y << "," << enemies[i].className << "," << std::endl;
    }
    else {
        file << "spawn_enemy((float)" << x << "*xScale, (float)" << y << "*yScale, xScale, yScale, " << enemies[i].className << ", &enemies);" << std::endl;
    }
  }

  file.close();
}

void Controller::levelSelect(bool edit) {
  std::vector<std::string> level_list; //to hold the list of level names.
  if(edit) level_list.push_back("new_level");

  level_list.push_back("level_01");
  level_list.push_back("level_02");

  int cursor_pos = 0; //cursor position.
  int y_offset = 100*yScale; //for scrolling list.

  view.readFolder("assets/levels/", &level_list);

  int list_size = static_cast<int>(level_list.size());

  int frame = 0;
  int frameTicker = 0;

  while(gamestatus == STATUS_LEVEL_SEL) {
    frameTicker++;
    if(frameTicker > 5) {
        frameTicker = 0;
        frame += 1;
        if(frame > 11) frame = 0;
    }

    if(view.getWindowStatus()) break;

    view.drawTexture(0,0,frame, WHITE);


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
        finalPath += ".txt";
        current_level = level_list[cursor_pos];
        if(!edit) {
            if(cursor_pos == 0) level_one();
            else if(cursor_pos == 1) level_two();
            else generate_Level(finalPath);
        }
        else {
            if(cursor_pos == 1) level_one();
            else if(cursor_pos == 2) level_two();
            else generate_Level(finalPath);
        }
        if(edit)
          gamestatus = STATUS_EDIT;
        else
          gamestatus = STATUS_PLAYING;
      }
      if(gamepad.back_released) {
          gamestatus = STATUS_MENU;
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
    while(y_offset + (cursor_pos*50*yScale) < 100*yScale) {
      y_offset += 50*yScale;
    }

    view.startFrame();

    view.drawText("Select Level", (view.getScreenWidth() / 2) - (MeasureText("Select Level", 70*yScale) / 2), 20*yScale, 70*yScale, RED);

    for(int i = 0; i < list_size; i++) {
      if(y_offset + i*50*yScale > 20*yScale + 70*yScale) {
          view.drawText(level_list[i], (view.getScreenWidth() / 2) - (MeasureText(level_list[i].c_str(), 50*yScale) / 2) , y_offset + i*50*yScale, 50*yScale, (cursor_pos == i) ? (Color) {0,0,255,255} : (Color) {255,255,255,255});
      }
    }

    //view.drawText(points_string, 20, 20, 40, WHITE);

    view.endFrame();
  }

  if(edit) {
    for(int i = 0; i < enemies.size(); i++) {
      enemies[i].rect.y += view.getScreenHeight();
    }
  }
}
