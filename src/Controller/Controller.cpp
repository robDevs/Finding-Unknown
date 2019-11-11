#include "Controller.h"

Controller::Controller() {
  gamestatus = STATUS_MENU;
  xScale = 1;
  yScale = 1;

  view.initView(2560,1440, &xScale, &yScale);
  SetWindowPosition(1440, 0);

  gameLoop();
}
void Controller::entityLoop() {
  for(int i = 0; i < (int) entities.size() - 1; i++) {
    entities[i].move();
  }
}
void Controller::gameLoop() {
  while(!view.getWindowStatus()) {
      doIntro();
    if(gamestatus == STATUS_MENU) {
      doMenu();
    }
    if(gamestatus == STATUS_PLAYING) {
      doGame();
    }
  }
  view.freeTextures();
}

void Controller::doIntro() {
    int frameBeam = 0;
    int frameStars = 0;
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
          if(frameBeam > 1) frameBeam = 0;
          if(frameStars > 12) frameStars = 0;
      }

      beamPos += 20.0*xScale;

      if(beamPos + view.getTextureWidth(BEAM_A) > view.getScreenWidth() && titleFade < 255) {
          titleFade+=5;
          if(titlePos > view.getScreenHeight()/16*yScale) {
              titlePos -= 5*yScale;
          }
      }

      if(titlePos <= view.getScreenHeight()/16*yScale && titleFade >= 255 && frameStars == 1) {
          break;
      }


      view.startFrame();

      if(view.getWindowStatus()) break;

      view.drawTexture(0,0,frameStars, WHITE);

      if(frameBeam == 0) {
          view.drawTexture(beamPos,view.getScreenHeight()/2 - view.getTextureHeight(BEAM_A), BEAM_A, WHITE);
      }
      else
        view.drawTexture(beamPos,view.getScreenHeight()/2 - view.getTextureHeight(BEAM_B), BEAM_B, WHITE);

      view.drawTexture(view.getScreenWidth()/2 - view.getTextureWidth(TITLE_)/2, titlePos, TITLE_, (Color){255,255,255,titleFade});

      view.endFrame();
    }
}

void Controller::doMenu() {
  int cursor_pos = 0;
  int frame = 0;
  int frameTicker = 0;
  while(gamestatus == STATUS_MENU) {

    control_info gamepad = view.getControlInfo();
    if(gamepad.enter_released) {
        if(GetScreenWidth() > 1280) {
            view.initView(1280,720, &xScale, &yScale);
        }
        else {
            view.initView(GetMonitorWidth(0),GetMonitorHeight(0), &xScale, &yScale);
        }
        while(!IsWindowReady()) {}
        //ToggleFullscreen();
        view.loadTextures(xScale, yScale);
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

    view.drawTexture(view.getScreenWidth()/2 - view.getTextureWidth(TITLE_)/2, 0 + view.getScreenHeight()/16*yScale, TITLE_, WHITE);

    DrawText(FormatText("DETECTED AXIS [%i]:", GetGamepadAxisCount(GAMEPAD_PLAYER1)), 10, 50, 10, WHITE);

    for (int i = 0; i < GetGamepadAxisCount(GAMEPAD_PLAYER1); i++)
    {
        DrawText(FormatText("AXIS %i: %.02f", i, GetGamepadAxisMovement(GAMEPAD_PLAYER1, i)), 20, 70 + 20*i, 10, WHITE);
    }

    if (GetGamepadButtonPressed() != -1) DrawText(FormatText("DETECTED BUTTON: %i", GetGamepadButtonPressed()), 10, 430, 10, WHITE);
    else DrawText("DETECTED BUTTON: NONE", 10, 430, 10, GRAY);


    view.endFrame();
  }
}

void Controller::doGame() {

}
