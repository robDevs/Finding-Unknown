#include "Model.h"

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
//---------------------------------------------
// End Entity class functions.
//---------------------------------------------


//---------------------------------------------
// Entity call back functions go here.
//---------------------------------------------

Entity *self;

void player_update() {
  self->xPos += self->xVel;
  self->yPos += self->yVel;

  if(self->xVel < 0) self->frame = 0;
  if(self->xVel == 0) self->frame = 1;
  if(self->xVel > 0) self->frame = 2;
}
