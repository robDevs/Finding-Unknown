#ifndef _MODEL_
#define _MODEL

#include <string>
#include <vector>
#include <cstdlib>

#define ENTITY_KEEP       0  //keep the ent
#define ENTITY_REMOVE     1  //Remove the ent
#define ENTITY_DESTROY    2  //Spawn explosion at ent, give points from ent, remove ent.
#define ENTITY_SHOOT      3

#define BASIC_BULLET      0
#define ENEMY_BULLET      1
#define PLAYER_PLAYER     2

typedef struct {
    int x;
    int y;
    int width;
    int height;
}rectangle;

extern int screenWidth_model; //assigned by controller.cpp
extern int screenHeight_model;

class Entity {
public:
    Entity();
    float xVel; //Moves the entity.
    float yVel;
    int frame; //sets the frame for the spritesheet.
    rectangle rect; //hit box, and position.
    int className; //Name of the entity. can be used to check what it is.
    int textureName; //Name of the texture or sprite sheet. Check view.h for definitions.
    int status; //status of the ent. Used for checking what to do with the entity.
    int health; //...
    float xScale; // scale factor, multiply x,y-Vel by this when moving.
    float yScale;
    int points; //how many points this ent gives.
    int timer; //a timer. increase it every frame. Use it for things. 60 frames = 1 second.
    int timer1;
    int timer2;
    int lives;

    void (*hit)(); // A hit function. if defined and not bullet, called every frame upon certain collisions.
    void (*update)(); // Logic / update function. if defined called every frame. syntax is update = &func; where func is a defined function.
    void (*shoot)(std::vector<Entity> *ent_list);
    void setName(int name);
    void setTextureName(int name);
    void setX(float x);
    void setY(float y);
    void setXvel(float vel);
    void setYvel(float vel);
    void setRect(int w, int h);
    void move();
    float getX();
    float getY();
    rectangle getRect();
    int getStatus();
    int getHealth();
    int getFrame();
};

extern bool checkCollision(rectangle a, rectangle b);
extern bool checkCollisionPointRect(int pX, int pY, rectangle rect);

//----------------------------------------------
//Entity functions.
//Used as logic for entities.
//----------------------------------------------

//Afte the entity has been created it is passed to this function which adds it to a vector from controller.cpp
extern void finalize_entity(Entity target, std::vector<Entity> ent_list);

extern Entity *self; //Before an entities update or hit function is called self is assigned to that entity.
extern Entity *other;
extern Entity *player_pointer; //special pointer for the player entity.

//Update, hit, and spawn functions.
//First create The update and hit functions. Then a spawn function.
//Spawn function should always take scales and vector<Entity> arguments.
//If you don't define a hit a update func for an ent it simply wont be called by controller.cpp

extern void player_update();
extern void bullet_basic_update();
extern void basic_enemy_update();
extern void adv_enemy_update();
extern void tracker_enemy_update();
extern void bomber_enemy_update();
extern void fighter_update();

extern void basic_bullet_hit();
extern void player_hit();
extern void enemy_bullet_hit();
extern void enemy_hit();

extern void spawn_enemy(int x, int y, float xScale, float yScale, int type, std::vector<Entity> *ent_list);
extern void spawn_bullet(int x, int y, int type, float xScale, float yScale, std::vector<Entity> *ent_list);

extern void enemy_shoot(std::vector<Entity> *ent_list);

extern void burner_update();
extern void explosion_update();
extern void beam_burst_update();
extern void spawn_explosion(int x, int y, int type, std::vector<Entity> *ent_list);

#endif
