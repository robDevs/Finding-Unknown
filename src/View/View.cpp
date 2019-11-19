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

void sprite_sheet::setFrames(std::string path, float xScale, float yScale) {
    float x = 0;
    float y = 0;

    Image tempImage;

    //empty the frames vector #spritesheet scaling bug fix attempt 1
    while(!frames.empty()) {
        frames.pop_back();
    }

    tempImage = LoadImage(path.c_str());
    ImageResizeNN(&tempImage, (float)tempImage.width * xScale, (float)tempImage.height * yScale);
    texture = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    float x_frames = (float) texture.width / (frameWidth * xScale);
    float y_frames = (float) texture.height / (frameHeight * yScale);
    std::cout << "-----------frames: " <<  x_frames * y_frames << std::endl;
    for(int i = 0; i < x_frames * y_frames; i++) {
        frame currentFrame;
        currentFrame.x = x;
        currentFrame.y = y;
        currentFrame.width = frameWidth*xScale;
        currentFrame.height = frameHeight*yScale;

        frames.push_back(currentFrame);

        x += frameWidth*xScale;
        if(x >= texture.width) {
            x = 0;
            y += frameHeight*xScale;
        }
    }
}

void sprite_sheet::unloadTexture() {
  UnloadTexture(texture);
}

void View::initView(int width, int height, float *xScale, float *yScale) {
  screenWidth = width;
  screenHeight = height;

  *xScale = screenWidth/1280.00;
  *yScale = screenHeight/720.00;


  if(!IsWindowReady()) InitWindow(screenWidth, screenHeight, "Finding Unknown");
  else SetWindowSize(width, height);

  SetWindowPosition(0, 0);

  freeTextures();

  loadTextures(*xScale, *yScale);

  sprite_sheets[0].frameWidth = 400;
  sprite_sheets[0].frameHeight = 400;
  sprite_sheets[0].setFrames("assets/BurstEffect/Burst_SpriteSheet.png", *xScale, *yScale);

  sprite_sheets[1].frameWidth = 150;
  sprite_sheets[1].frameHeight = 200;
  sprite_sheets[1].setFrames("assets/Ships/Main/Main.png", *xScale, *yScale);

  SetTargetFPS(60);

  //SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
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
    for(int i = 0; i < 2; i++) {
      sprite_sheets[i].unloadTexture();
    }
}

void View::startFrame() {
  BeginDrawing();
  ClearBackground(BLACK);
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

void View::drawSprite(int x, int y, int spriteSheet, int frame, Color color) {
    Rectangle rect;
    rect.x = sprite_sheets[spriteSheet].frames[frame].x;
    rect.y = sprite_sheets[spriteSheet].frames[frame].y;
    rect.width = sprite_sheets[spriteSheet].frames[frame].width;
    rect.height = sprite_sheets[spriteSheet].frames[frame].height;
    DrawTextureRec(sprite_sheets[spriteSheet].texture, rect, (Vector2) {x, y}, color);
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
    return GetScreenWidth();
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
