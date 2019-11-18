#ifndef _MODEL_
#define _MODEL

#include "raylib.h"
#include <string>

class Entity {
private:
    int className;
    int textureName;
    float speed;
    bool solid;
    int moveType;
    int status;
    int health;
    Rectangle rect;
public:
    float xPos;
    float yPos;
    float xVel;
    float yVel;
    int frame;
    void (*hit)();
    void (*update)();
    void setName(int name);
    void setTextureName(int name);
    void setX(float x);
    void setY(float y);
    void setXvel(float vel);
    void setYvel(float vel);
    void setSpeed(float speed);
    void move();
    float getX();
    float getY();
    bool getSolid();
    Rectangle getRect();
    int getMoveType();
    int getStatus();
    int getHealth();
    int getFrame();
};

extern Entity *self;
extern void player_update();

#endif
