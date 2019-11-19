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
    rect.x = x;
}
void Entity::setY(float y) {
    rect.y = y;
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
  rect.x += xVel;
  rect.y += yVel;
}
float Entity::getX() {
    return rect.x;
}
float Entity::getY() {
    return rect.y;
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
  self->rect.x += self->xVel;
  self->rect.y += self->yVel;

  if(self->xVel < 0) self->frame = 0;
  if(self->xVel == 0) self->frame = 1;
  if(self->xVel > 0) self->frame = 2;
}

void test_enemy_update() {
  self->rect.x += self->xVel;
  self->rect.y += self->yVel;

  if(self->rect.y + self->rect.height > screenHeight_model) {
    self->status = ENTITY_REMOVE;
  }

  if(self->rect.y < 0) {
    self->yVel = 5;
    self->xVel = 0;
  }
}
void spawn_test_enemy(int x, int y, std::vector<Entity> *ent_list) {
  Entity new_entity;

  new_entity.update = &test_enemy_update;
  new_entity.health = 1;
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.yVel = 5;
  new_entity.rect.width = 50;
  new_entity.rect.height = 50;
  new_entity.status = ENTITY_KEEP;

  finalize_entity(new_entity, ent_list);
}
