#include "Model.h"

int screenWidth_model = 0;
int screenHeight_model = 0;

//---------------------------------------------
// Entity class functions.
//---------------------------------------------
void Entity::setName(int name) {
    className = name;
}
void Entity::setTextureName(int name) {
    textureName = name;
}
void Entity::setX(float x) {
    xPos = x;
}
void Entity::setY(float y) {
    yPos = y;
}
void Entity::setXvel(float vel) {
    xVel = vel;
}
void Entity::setYvel(float vel) {
    yVel = vel;
}
void Entity::setSpeed(float speed) {
    this->speed = speed;
}
void Entity::move() {
  xPos += xVel;
  yPos += yVel;
}
float Entity::getX() {
    return xPos;
}
float Entity::getY() {
    return yPos;
}
bool Entity::getSolid() {
    return solid;
}
Rectangle Entity::getRect() {
    return rect;
}
int Entity::getMoveType() {
    return moveType;
}
int Entity::getStatus() {
    return status;
}
int Entity::getHealth() {
    return health;
}
int Entity::getFrame() {
  return frame;
}

void Entity::setRect(int w, int h) {
  rect.width = w;
  rect.height = h;
}
//---------------------------------------------
// End Entity class functions.
//---------------------------------------------


//---------------------------------------------
// Entity call back functions go here.
//---------------------------------------------

Entity *self;

void finalize_entity(Entity target, std::vector<Entity> *ent_list) {
  ent_list->push_back(target);
}

void player_update() {
  self->xPos += self->xVel;
  self->yPos += self->yVel;

  if(self->xVel < 0) self->frame = 0;
  if(self->xVel == 0) self->frame = 1;
  if(self->xVel > 0) self->frame = 2;
}

void test_enemy_update() {
  self->xPos += self->xVel;
  self->yPos += self->yVel;

  self->rect.x = self->xPos;
  self->rect.y = self->yPos;

  if(self->rect.y + self->rect.height > screenHeight_model) {
    self->status = ENTITY_REMOVE;
  }

  if(self->yPos < 0) {
    self->yVel = 2;
    self->xVel = 0;
  }
}
void spawn_test_enemy(int x, int y, std::vector<Entity> *ent_list) {
  Entity new_entity;

  new_entity.update = &test_enemy_update;
  new_entity.health = 1;
  new_entity.xPos = x;
  new_entity.yPos = y;
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.yVel = 2;
  new_entity.rect.width = 20;
  new_entity.rect.height = 20;
  new_entity.status = ENTITY_LIVING;

  finalize_entity(new_entity, ent_list);
}
