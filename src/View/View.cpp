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

void View::initView(int width, int height, float *xScale, float *yScale) {
  screenWidth = width;
  screenHeight = height;

  *xScale = screenWidth/1280;
  *yScale = screenHeight/720;


  if(!IsWindowReady()) InitWindow(screenWidth, screenHeight, "Finding Unknown");
  else SetWindowSize(width, height);

  SetWindowPosition(0, 0);

  loadTextures(*xScale, *yScale);

  SetTargetFPS(60);
}

void View::setScale(float width, float height,float *xScale, float *yScale) {
    screenWidth = width;
    screenHeight = height;

    *xScale = width/1280.00;
    *yScale = height/720.00;
}

bool View::loadTextures(float xScale, float yScale) {
    Image tempImage;

    for(int i = 0; i < 13; i++) {
        UnloadTexture(textures[i]);
    }

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00000.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[0] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00001.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[1] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00002.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[2] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00003.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[3] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00004.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[4] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00005.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[5] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00006.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[6] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00007.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[7] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00008.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[8] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00009.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[9] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00010.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[10] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00011.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[11] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/BG/NoAlpha/BG_00012.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[12] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/Logo.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[13] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/Beam/Beam_A.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[14] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/TitleScreen/Beam/Beam_A.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[15] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
}

void View::freeTextures() {
    for(int i = 0; i < 15; i++) {
        UnloadTexture(textures[i]);
    }
}

void View::startFrame() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
}

void View::endFrame() {
  EndDrawing();
}

bool View::getWindowStatus() {
  return WindowShouldClose();
}

void View::drawTexture(int x, int y, int texture, Color color) {
  DrawTexture(textures[texture], x, y, color);
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

float View::getScreenWidth() {
    return screenWidth;
}

float View::getScreenHeight() {
    return screenHeight;
}

int View::getTextureWidth(int i) {
    return textures[i].width;
}

int View::getTextureHeight(int i) {
    return textures[i].height;
}
