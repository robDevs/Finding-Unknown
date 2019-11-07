#include "View.h"

void control_info::init() {
  up_pressed = false;
  up_held = false;
  up_released = false;

  down_pressed = false;
  down_held = false;
  down_released = false;

  left_pressed = false;
  left_held = false;
  left_released = false;

  right_pressed = false;
  right_held = false;
  right_released = false;

  enter_pressed = false;
  enter_held = false;
  enter_released = false;

  back_pressed = false;
  back_held = false;
  back_released = false;
}

void View::initView(int width, int height) {
  screenWidth = width;
  screenHeight = height;

  InitWindow(screenWidth, screenHeight, "Finding Unknown");
  SetTargetFPS(60);
}

bool View::getWindowStatus() {
  return WindowShouldClose();
}

void View::drawTexture(int x, int y, Texture2D texture) {
  DrawTexture(texture, x, y, WHITE);
}

void View::drawText(std::string text, int x, int y, int size, Color color) {
  DrawText(text.c_str(), x, y, size, color);
}
control_info View::getControlInfo() {
  control_info gamepad;
  gamepad.init();
  //Read the input from usb gamepad.
  if (IsGamepadAvailable(GAMEPAD_PLAYER1)) {
    if(IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_LEFT)) {
      gamepad.left_pressed = true;
    }

    if(IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_LEFT)) {
      gamepad.left_held = true;
    }

    if(IsGamepadButtonReleased(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_LEFT)) {
      gamepad.left_released = true;
    }

    if(IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_RIGHT)) {
      gamepad.right_pressed = true;
    }

    if(IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_RIGHT)) {
      gamepad.right_held = true;
    }

    if(IsGamepadButtonReleased(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_RIGHT)) {
      gamepad.right_released = true;
    }

    if(IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_UP)) {
      gamepad.up_pressed = true;
    }

    if(IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_UP)) {
      gamepad.up_held = true;
    }

    if(IsGamepadButtonReleased(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_UP)) {
      gamepad.up_released = true;
    }

    if(IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_DOWN)) {
      gamepad.down_pressed = true;
    }

    if(IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_DOWN)) {
      gamepad.down_held = true;
    }

    if(IsGamepadButtonReleased(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_DOWN)) {
      gamepad.down_released = true;
    }

    if(IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_B)) {
      gamepad.enter_pressed = true;
    }

    if(IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_B)) {
      gamepad.enter_held = true;
    }

    if(IsGamepadButtonReleased(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_B)) {
      gamepad.enter_released = true;
    }

    if(IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_A)) {
      gamepad.back_pressed = true;
    }

    if(IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_A)) {
      gamepad.back_held = true;
    }

    if(IsGamepadButtonReleased(GAMEPAD_PLAYER1, GAMEPAD_BUFFALOSNES_BUTTON_A)) {
      gamepad.back_released = true;
    }
  }
  //Read the input from the keyboard.
  if(IsKeyPressed(KEY_W)) {
    gamepad.up_pressed = true;
  }

  if(IsKeyDown(KEY_W)) {
    gamepad.up_held = true;
  }

  if(IsKeyReleased(KEY_W)) {
    gamepad.up_released = true;
  }

  if(IsKeyPressed(KEY_A)) {
    gamepad.left_pressed = true;
  }

  if(IsKeyDown(KEY_A)) {
    gamepad.left_held = true;
  }

  if(IsKeyReleased(KEY_A)) {
    gamepad.left_released = true;
  }

  if(IsKeyPressed(KEY_S)) {
    gamepad.down_pressed = true;
  }

  if(IsKeyDown(KEY_S)) {
    gamepad.down_held = true;
  }

  if(IsKeyReleased(KEY_S)) {
    gamepad.down_released = true;
  }

  if(IsKeyPressed(KEY_D)) {
    gamepad.right_pressed = true;
  }

  if(IsKeyDown(KEY_D)) {
    gamepad.right_held = true;
  }

  if(IsKeyReleased(KEY_D)) {
    gamepad.right_released = true;
  }

  if(IsKeyPressed(KEY_ENTER)) {
    gamepad.enter_pressed = true;
  }

  if(IsKeyDown(KEY_ENTER)) {
    gamepad.enter_held = true;
  }

  if(IsKeyReleased(KEY_ENTER)) {
    gamepad.enter_released = true;
  }

  if(IsKeyPressed(KEY_BACKSPACE)) {
    gamepad.back_pressed = true;
  }

  if(IsKeyDown(KEY_BACKSPACE)) {
    gamepad.back_held = true;
  }

  if(IsKeyReleased(KEY_BACKSPACE)) {
    gamepad.back_released = true;
  }

  return gamepad;
}
