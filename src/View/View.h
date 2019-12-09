#ifndef _VIEW_
#define _VIEW_

#include "raylib.h"

#include <string>
#include <vector>

#include <iostream>


//-------------------------------------------------------------------------------------------
// Define info about Buffalo snes style controller. Tested on ubuntu linux
//-------------------------------------------------------------------------------------------

#define GAMEPAD_BUFFALOSNES_BUTTON_B            7
#define GAMEPAD_BUFFALOSNES_BUTTON_A            6
#define GAMEPAD_BUFFALOSNES_BUTTON_Y            8
#define GAMEPAD_BUFFALOSNES_BUTTON_X            5
#define GAMEPAD_BUFFALOSNES_BUTTON_UP           1
#define GAMEPAD_BUFFALOSNES_BUTTON_DOWN         3
#define GAMEPAD_BUFFALOSNES_BUTTON_LEFT         4
#define GAMEPAD_BUFFALOSNES_BUTTON_RIGHT        2
#define GAMEPAD_BUFFALOSNES_BUTTON_START        15
#define GAMEPAD_BUFFALOSNES_BUTTON_SELECT       13
#define GAMEPAD_BUFFALOSNES_BUTTON_L            9
#define GAMEPAD_BUFFALOSNES_BUTTON_R            11

/*
#define GAMEPAD_BUFFALOSNES_BUTTON_B            1
#define GAMEPAD_BUFFALOSNES_BUTTON_A            0
#define GAMEPAD_BUFFALOSNES_BUTTON_Y            3
#define GAMEPAD_BUFFALOSNES_BUTTON_X            2
#define GAMEPAD_BUFFALOSNES_BUTTON_UP           axis1 -1
#define GAMEPAD_BUFFALOSNES_BUTTON_DOWN         axis1 +1
#define GAMEPAD_BUFFALOSNES_BUTTON_LEFT         axis0 -1
#define GAMEPAD_BUFFALOSNES_BUTTON_RIGHT        axis0 +1
#define GAMEPAD_BUFFALOSNES_BUTTON_START        7
#define GAMEPAD_BUFFALOSNES_BUTTON_SELECT       6
#define GAMEPAD_BUFFALOSNES_BUTTON_L            4
#define GAMEPAD_BUFFALOSNES_BUTTON_R            5
*/

#define BG_00001        1
#define BG_00002        2
#define BG_00003        3
#define BG_00004        4
#define BG_00005        5
#define BG_00006        6
#define BG_00007        7
#define BG_00008        8
#define BG_00009        9
#define BG_00010        10
#define BG_00011        11
#define BG_00012        12
#define TITLE_          13
#define BEAM_A          14
#define BEAM_B          15
#define BG_SCROLL_01    16
#define BG_SCROLL_02    17
#define BG_SCROLL_03    18
#define BG_SCROLL_04    19
#define BG_SCROLL_05    20
#define BG_SCROLL_06    21

#define BURST_SPRITESHEET             0
#define PLAYER_SPRITESHEET            1
#define BASIC_BULLET_SPRITE_SHEET     2
#define BASIC_CHARGE_SPRITE_SHEET     3
#define HELIX_BEAM_SPRITE_SHEET       4
#define BURNER_SPRITE_SHEET           5
#define BASIC_ENEMY_SPRITE_SHEET      6
#define ADV_ENEMY_SPRITE_SHEET        7
#define HARD_ENEMY_SPRITE_SHEET       8
#define SHOT_HIT_SPRITE_SHEET         9
#define FIGHTER_SPRITESHEET           10

#define SONG_ALERT                    0
#define SONG_ARCADE                   1
#define SONG_ASTEROID                 2
#define SONG_EVIL                     3
#define SONG_NUKE                     4
#define SONG_RED                      5
#define SFX_EXP                       6
#define SFX_SHOOT                     7
#define SFX_HIT                       8


#define MAX_INPUT_CHARS     30

//-------------------------------------------------------------------------------------------
// Control info struct.
// Allows the controller (controller.h) not worry about the button and only the action.
//-------------------------------------------------------------------------------------------
struct control_info {
  void init();
  bool up_pressed;
  bool up_held;
  bool up_released;

  bool down_pressed;
  bool down_held;
  bool down_released;

  bool left_pressed;
  bool left_held;
  bool left_released;

  bool right_pressed;
  bool right_held;
  bool right_released;

  bool enter_pressed;
  bool enter_held;
  bool enter_released;

  bool back_pressed;
  bool back_held;
  bool back_released;

  bool pause_pressed;
  bool pause_held;
  bool pause_released;

  bool space_pressed;
  bool space_held;
  bool space_released;
};

struct frame {
    float x;
    float y;
    float width;
    float height;
};

struct sprite_sheet {
    Texture2D texture;
    float frameWidth;
    float frameHeight;
    std::vector<frame> frames;
    void setFrames(std::string path, float xScale, float yScale);
    void unloadTexture();
};

struct textBox{
    char message[MAX_INPUT_CHARS + 1];
    Rectangle rect;
    int max;
    bool mouseOnText;
    int letterCount;
    int framesCounter;
    textBox(int x, int y, int w, int h, int max);
    void update();
    void draw();
    void setMessage(const char newMessage[MAX_INPUT_CHARS + 1]);
};

//------------------------------------------------------------------------------------------
// The view handles drawing things and getting input from user.
//------------------------------------------------------------------------------------------
class View {
private:
  float screenWidth;
  float screenHeight;
  Texture2D textures[22];
  sprite_sheet sprite_sheets[11];

  Sound sounds[9];

public:
  void initView(int width, int height, float *xScale, float *yScale); // Create a window with the given attributes.
  void setFullScreen(float *xScale, float *yScale);
  void setScale(float width, float height,float *xScale, float *yScale);
  void startFrame();
  void endFrame();
  bool getWindowStatus(); // check if the window should close (If x thing was pressed. )
  bool loadTextures(float xScale, float yScale); //return false if textures failed to load.
  void freeTextures();
  void drawTexture(int x, int y, int texture, Color color); // Draws a texture at the given pos.
  void drawText(std::string text, int x, int y, int size, Color color); // draws text at given pos.
  control_info getControlInfo(); // read input and return a control_info struct.

  void drawSprite(int x, int y, int spriteSheet, int frame, Color color);

  float getScreenWidth();
  float getScreenHeight();

  int getTextureWidth(int i);
  int getTextureHeight(int i);

  void drawRectBorders(int x, int y, int h, int w, Color color_outline, Color color_fill);

  void readFolder(std::string path, std::vector<std::string> *list);
  void freeSounds();

  void playSound(int Sound);
  void stodSound(int Sound);
  void loopSound(int Sound);
};

#endif
