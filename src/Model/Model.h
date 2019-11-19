#ifndef _MODEL_
#define _MODEL

#include "raylib.h"
#include <string>
#include <vector>
#include <cstdlib>

#define ENTITY_LIVING     0
#define ENTITY_REMOVE     1
#define ENTITY_DESTROY    2

extern int screenWidth_model;
extern int screenHeight_model;

class Entity {
public:
    float xPos;
    float yPos;
    float xVel;
    float yVel;
    int frame;
    Rectangle rect;
    int className;
    int textureName;
    float speed;
    bool solid;
    int moveType;
    int status;
    int health;

    void (*hit)();
    void (*update)();
    void setName(int name);
    void setTextureName(int name);
    void setX(float x);
    void setY(float y);
    void setXvel(float vel);
    void setYvel(float vel);
    void setSpeed(float speed);
    void setRect(int w, int h);
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

extern void finalize_entity(Entity target, std::vector<Entity> ent_list);

extern Entity *self;
extern void player_update();

extern void test_enemy_update();
extern void spawn_test_enemy(int x, int y, std::vector<Entity> *ent_list);

#endif
