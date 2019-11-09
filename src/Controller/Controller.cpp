#include "Controller.h"

Controller::Controller() {
  gamestatus = STATUS_MENU;

  view.initView(1280,720);

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
}

void Controller::doMenu() {
  int cursor_pos = 0;
  while(gamestatus == STATUS_MENU) {
    view.startFrame();

    if(view.getWindowStatus()) break;

    control_info gamepad = view.getControlInfo();

    view.drawText("Do menu stuff here\ncheck for button presses, act according to cursor pos\ndraw stuff", 450, 450, 20, BLACK);

    view.endFrame();
  }
}

void Controller::doGame() {

}
