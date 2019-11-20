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
Entity *other;
Entity *player;

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

void bullet_basic_update() {
  self->rect.x += self->xVel;
  self->rect.y += self->yVel;

  self->timer++;

  if(self->timer > 30) {
    self->timer = 0;
    self->frame++;
  }

  if(self->frame > 1) {
    self->frame = 0;
  }

  if(self->rect.y + self->rect.height < 0 ||
     self->rect.x + self->rect.width < 0 ||
     self->rect.x > screenWidth_model ||
     self->health <= 0)
     {
        self->status = ENTITY_REMOVE;
     }
}
void basic_bullet_hit() {
  self->health -= 1;
}
void spawn_bullet(int x, int y, int type, std::vector<Entity> *ent_list){
  Entity new_entity;

  if(type == BASIC_BULLET) {
    new_entity.update = &bullet_basic_update;
    new_entity.yVel = -7;
    new_entity.health = 1;
    new_entity.className = type;
    new_entity.textureName = 2;
    new_entity.hit = &basic_bullet_hit;
    new_entity.timer = 0;
  }
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.xVel = 0;
  new_entity.rect.width = 10;
  new_entity.rect.height = 10;
  new_entity.status = ENTITY_KEEP;
  new_entity.frame = 0;

  finalize_entity(new_entity, ent_list);
}

void test_enemy_update() {
  self->rect.x += self->xVel;
  self->rect.y += self->yVel;

  if(self->rect.y > screenHeight_model ||
     self->rect.x < 0 ||
     self->rect.x + self->rect.width > screenWidth_model)
     {
        self->status = ENTITY_REMOVE;
     }

  if(self->health <= 0) {
    self->status = ENTITY_DESTROY;
  }

  if(self->rect.y < 0) {
    self->yVel = 5;
    self->xVel = 0;
  }
}
void test_enemy_hit() {
  if(other->className == BASIC_BULLET) {
    self->health -= 1;
    other->status = ENTITY_REMOVE;
  }
}
void spawn_test_enemy(int x, int y, std::vector<Entity> *ent_list) {
  Entity new_entity;

  new_entity.update = &test_enemy_update;
  new_entity.hit = &test_enemy_hit;
  new_entity.health = 1;
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.yVel = 5;
  new_entity.rect.width = 50;
  new_entity.rect.height = 50;
  new_entity.status = ENTITY_KEEP;
  new_entity.points = 26;

  finalize_entity(new_entity, ent_list);
}
