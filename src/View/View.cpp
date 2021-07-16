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

  pause_pressed = false;
  pause_held = false;
  pause_released = false;

  space_pressed = false;
  space_held = false;
  space_released = false;
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

textBox::textBox(int x, int y, int w, int h, int max) {
    for(int i = 0; i < MAX_INPUT_CHARS + 1; i++) {
        message[i] = '\0';
    }
    this->rect.x = x;
    this->rect.y = y;
    this->rect.width = w;
    this->rect.height = h;
    this->max = max;
    this->mouseOnText = false;
    this->letterCount = 0;
    this->framesCounter = 0;
}

void textBox::update() {

    if (CheckCollisionPointRec(GetMousePosition(), rect)) mouseOnText = true;
    else mouseOnText = false;

    if (mouseOnText)
    {
        int key = GetKeyPressed();

        // NOTE: Only allow keys in range [32..125]
        if ((key >= 32) && (key <= 125) && (letterCount < max))
        {
            message[letterCount] = (char)key;
            letterCount++;
        }

        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0)
        {
            letterCount--;
            message[letterCount] = '\0';

            if (letterCount < 0) letterCount = 0;
        }
    }

    if (mouseOnText) framesCounter++;
    else framesCounter = 0;
}

void textBox::draw() {
    DrawRectangleRec(rect, LIGHTGRAY);

    if (mouseOnText) DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, RED);
    else DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, DARKGRAY);

    DrawText(message, rect.x + 5, rect.y + 8, 20, RED);

    if (mouseOnText)
    {
        if (letterCount < MAX_INPUT_CHARS)
        {
            // Draw blinking underscore char
            if (((framesCounter/20)%2) == 0) DrawText("_", rect.x + 8 + MeasureText(message, 20), rect.y + 12, 20, VIOLET);
        }
    }
}

void textBox::setMessage(const char newMessage[MAX_INPUT_CHARS + 1]) {
  for(int i = 0; i < MAX_INPUT_CHARS + 1; i++) {
    if(newMessage[i] == ' ' || newMessage[i] == NULL || !newMessage[i])
      break;
    message[i] = newMessage[i];
    letterCount++;
  }
}

void View::initView(int width, int height, float *xScale, float *yScale) {
  screenWidth = width;
  screenHeight = height;

  *xScale = screenWidth/1280.00;
  *yScale = screenHeight/720.00;

  if(!IsAudioDeviceReady()) InitAudioDevice();


  if(!IsWindowReady()) InitWindow(screenWidth, screenHeight, "Finding Unknown");
  else SetWindowSize(width, height);

  SetWindowPosition(0, 0);

  freeTextures();

  loadTextures(*xScale, *yScale);

  sprite_sheets[0].frameWidth = 400;
  sprite_sheets[0].frameHeight = 400;
  sprite_sheets[0].setFrames("assets/BurstEffect/Burst_SpriteSheet.png", *xScale, *yScale);

  sprite_sheets[1].frameWidth = 132;
  sprite_sheets[1].frameHeight = 161;
  sprite_sheets[1].setFrames("assets/Ships/Main/Main.png", *xScale, *yScale);

  sprite_sheets[2].frameWidth = 8;
  sprite_sheets[2].frameHeight = 22;
  sprite_sheets[2].setFrames("assets/Ships/Beams/BasicBeam.png", *xScale, *yScale);

  sprite_sheets[3].frameWidth = 12;
  sprite_sheets[3].frameHeight = 56;
  sprite_sheets[3].setFrames("assets/Ships/Beams/BasicBeamCharge.png", *xScale, *yScale);

  sprite_sheets[4].frameWidth = 22;
  sprite_sheets[4].frameHeight = 74;
  sprite_sheets[4].setFrames("assets/Ships/Beams/HelixBeam.png", *xScale, *yScale);

  sprite_sheets[5].frameWidth = 22;
  sprite_sheets[5].frameHeight = 11;
  sprite_sheets[5].setFrames("assets/Ships/Burners/BurnerRepeat.png", *xScale, *yScale);

  sprite_sheets[6].frameWidth = 150;
  sprite_sheets[6].frameHeight = 150;
  sprite_sheets[6].setFrames("assets/Ships/Enemy/Enemy_1_Spin.png", *xScale, *yScale);

  sprite_sheets[7].frameWidth = 150;
  sprite_sheets[7].frameHeight = 150;
  sprite_sheets[7].setFrames("assets/Ships/Enemy/Enemy_2_Spin.png", *xScale, *yScale);

  sprite_sheets[8].frameWidth = 150;
  sprite_sheets[8].frameHeight = 150;
  sprite_sheets[8].setFrames("assets/Ships/Enemy/Enemy_3_Spin.png", *xScale, *yScale);

  sprite_sheets[9].frameWidth = 50;
  sprite_sheets[9].frameHeight = 50;
  sprite_sheets[9].setFrames("assets/Ships/Beams/ShotHit.png", *xScale, *yScale);

  sprite_sheets[10].frameWidth = 146;
  sprite_sheets[10].frameHeight = 140;
  sprite_sheets[10].setFrames("assets/Ships/Enemy/Enemy_4.png", *xScale, *yScale);

  sounds[0] = LoadSound("assets/Audio/Music/Alert\ Bagel\ Danger\ BulletDarkness.mp3");
  sounds[1] = LoadSound("assets/Audio/Music/Arcade\ Geometry\ VEUSKEMINI.mp3");
  sounds[2] = LoadSound("assets/Audio/Music/Asteroid\ Panic\ Burn7.mp3");
  sounds[3] = LoadSound("assets/Audio/Music/Evil\ Intent\ IViqrr.mp3");
  sounds[4] = LoadSound("assets/Audio/Music/NUKE\ VEUSKEMINI.mp3");
  sounds[5] = LoadSound("assets/Audio/Music/Red\ Soil\ Greyvoice.mp3");
  sounds[6] = LoadSound("assets/Audio/Sfx/explosion.wav");
  sounds[7] = LoadSound("assets/Audio/Sfx/shoot.wav");
  sounds[8] = LoadSound("assets/Audio/Sfx/hit.wav");

  for(int i = 0; i < 8; i++) {
      SetSoundVolume(sounds[i], 0.1);
  }
  SetSoundVolume(sounds[7], 0.1);
  SetSoundVolume(sounds[8], 0.1);


  SetTargetFPS(60);

  //SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
}

void View::setFullScreen(float *xScale, float *yScale) {
    ToggleFullscreen();

    initView(GetScreenWidth(), GetScreenHeight(), xScale, yScale);
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

    tempImage = LoadImage("assets/BG/BG_01.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[16] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/BG/BG_02.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[17] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/BG/BG_03.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[18] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/BG/BG_04.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[19] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/BG/BG_05.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[20] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);

    tempImage = LoadImage("assets/BG/BG_06.png");
    ImageResizeNN(&tempImage, tempImage.width * xScale, tempImage.height * yScale);
    textures[21] = LoadTextureFromImage(tempImage);
    UnloadImage(tempImage);
}

void View::freeTextures() {
    for(int i = 0; i < 21; i++) {
        UnloadTexture(textures[i]);
    }
    for(int i = 0; i < 10; i++) {
      sprite_sheets[i].unloadTexture();
    }
}

void View::freeSounds() {
  for(int i = 0; i < 8; i++) {
    UnloadSound(sounds[i]);
  }
  CloseAudioDevice();
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
  if (IsGamepadAvailable(0)) {
    if(IsGamepadButtonPressed(0 , GAMEPAD_BUFFALOSNES_BUTTON_LEFT)) {
      gamepad.left_pressed = true;
    }

    if(IsGamepadButtonDown(0 , GAMEPAD_BUFFALOSNES_BUTTON_LEFT)) {
      gamepad.left_held = true;
    }

    if(IsGamepadButtonReleased(0 , GAMEPAD_BUFFALOSNES_BUTTON_LEFT)) {
      gamepad.left_released = true;
    }

    if(IsGamepadButtonPressed(0 , GAMEPAD_BUFFALOSNES_BUTTON_RIGHT)) {
      gamepad.right_pressed = true;
    }

    if(IsGamepadButtonDown(0 , GAMEPAD_BUFFALOSNES_BUTTON_RIGHT)) {
      gamepad.right_held = true;
    }

    if(IsGamepadButtonReleased(0 , GAMEPAD_BUFFALOSNES_BUTTON_RIGHT)) {
      gamepad.right_released = true;
    }

    if(IsGamepadButtonPressed(0 , GAMEPAD_BUFFALOSNES_BUTTON_UP)) {
      gamepad.up_pressed = true;
    }

    if(IsGamepadButtonDown(0 , GAMEPAD_BUFFALOSNES_BUTTON_UP)) {
      gamepad.up_held = true;
    }

    if(IsGamepadButtonReleased(0 , GAMEPAD_BUFFALOSNES_BUTTON_UP)) {
      gamepad.up_released = true;
    }

    if(IsGamepadButtonPressed(0 , GAMEPAD_BUFFALOSNES_BUTTON_DOWN)) {
      gamepad.down_pressed = true;
    }

    if(IsGamepadButtonDown(0 , GAMEPAD_BUFFALOSNES_BUTTON_DOWN)) {
      gamepad.down_held = true;
    }

    if(IsGamepadButtonReleased(0 , GAMEPAD_BUFFALOSNES_BUTTON_DOWN)) {
      gamepad.down_released = true;
    }

    if(IsGamepadButtonPressed(0 , GAMEPAD_BUFFALOSNES_BUTTON_B)) {
      gamepad.enter_pressed = true;
    }

    if(IsGamepadButtonDown(0 , GAMEPAD_BUFFALOSNES_BUTTON_B)) {
      gamepad.enter_held = true;
    }

    if(IsGamepadButtonReleased(0 , GAMEPAD_BUFFALOSNES_BUTTON_B)) {
      gamepad.enter_released = true;
    }

    if(IsGamepadButtonPressed(0 , GAMEPAD_BUFFALOSNES_BUTTON_A)) {
      gamepad.back_pressed = true;
    }

    if(IsGamepadButtonDown(0 , GAMEPAD_BUFFALOSNES_BUTTON_A)) {
      gamepad.back_held = true;
    }

    if(IsGamepadButtonReleased(0 , GAMEPAD_BUFFALOSNES_BUTTON_A)) {
      gamepad.back_released = true;
    }

    if(IsGamepadButtonPressed(0 , GAMEPAD_BUFFALOSNES_BUTTON_Y)) {
      gamepad.space_pressed = true;
    }

    if(IsGamepadButtonDown(0 , GAMEPAD_BUFFALOSNES_BUTTON_Y)) {
      gamepad.space_held = true;
    }

    if(IsGamepadButtonReleased(0 , GAMEPAD_BUFFALOSNES_BUTTON_Y)) {
      gamepad.space_released = true;
    }

    if(IsGamepadButtonPressed(0 , GAMEPAD_BUFFALOSNES_BUTTON_START)) {
      gamepad.pause_pressed = true;
    }

    if(IsGamepadButtonDown(0 , GAMEPAD_BUFFALOSNES_BUTTON_START)) {
      gamepad.pause_held = true;
    }

    if(IsGamepadButtonReleased(0 , GAMEPAD_BUFFALOSNES_BUTTON_START)) {
      gamepad.pause_released = true;
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

  if(IsKeyPressed(KEY_P)) {
    gamepad.pause_pressed = true;
  }

  if(IsKeyDown(KEY_P)) {
    gamepad.pause_held = true;
  }

  if(IsKeyReleased(KEY_P)) {
    gamepad.pause_released = true;
  }

  if(IsKeyPressed(KEY_SPACE)) {
    gamepad.space_pressed = true;
  }

  if(IsKeyDown(KEY_SPACE)) {
    gamepad.space_held = true;
  }

  if(IsKeyReleased(KEY_SPACE)) {
    gamepad.space_released = true;
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

void View::drawRectBorders(int x, int y, int w, int h, Color color_outline, Color color_fill) {
  DrawRectangle(x,y,w,h,color_fill);
  DrawRectangleLines(x,y,w,h,color_outline);
  DrawRectangleLines(x+1,y+1,w-2,h-2,color_outline);
}

void View::readFolder(std::string path, std::vector<std::string> *list) {
  int fileCount = 0;
  char **files = GetDirectoryFiles(path.c_str(), &fileCount);

  for(int i = 0; i < fileCount; i++) {
      std::string fileName = GetFileNameWithoutExt(files[i]);
      if(fileName.length() > 2)
        list->push_back(fileName);
  }

  ClearDirectoryFiles();
}

void View::playSound(int sound) {
  PlaySound(sounds[sound]);
}

void View::stodSound(int sound) {
  StopSound(sounds[sound]);
}

void View::loopSound(int sound) {
    if(!IsSoundPlaying(sounds[sound])) PlaySound(sounds[sound]);
}
