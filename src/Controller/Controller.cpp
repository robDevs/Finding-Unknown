#include "Controller.h"

Controller::Controller() {
  gamestatus = STATUS_MENU;
  xScale = 1;
  yScale = 1;

  view.initView(1280,720, &xScale, &yScale);

  gameLoop();
}
void Controller::entityLoop() {
  for(int i = 0; i < (int) entities.size() - 1; i++) {
    entities[i].move();
  }
}
void Controller::gameLoop() {
  while(!view.getWindowStatus()) {
    if(gamestatus == STATUS_MENU) {
      doMenu();
    }
    if(gamestatus == STATUS_PLAYING) {
      doGame();
    }
  }
  view.freeTextures();
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



    view.drawTexture(0,0,frame);

    int y = 200;
    for(int i = 0; i < GetMonitorCount(); i++) {
        std::string mName = "screen dimensions: ";
        mName += FormatText("%ix%i", GetMonitorWidth(0), GetMonitorHeight(0));

        view.drawText(FormatText("monitor: %ix%i", GetMonitorWidth(0), GetMonitorHeight(0)),200,190,20*((xScale+yScale) / 2),WHITE);
        view.drawText(FormatText("window: %ix%i", GetScreenWidth(), GetScreenHeight()),200,230,20*((xScale+yScale) / 2),WHITE);

        view.drawText(FormatText("scale: %lfx%lf", xScale, yScale),200,260,20*((xScale+yScale) / 2),BLUE);
        y += 40;
    }

    view.endFrame();
  }
}

void Controller::doGame() {

}
