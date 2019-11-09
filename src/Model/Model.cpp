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
//---------------------------------------------
// End Entity class functions.
//---------------------------------------------


//---------------------------------------------
// Entity call back functions go here.
//---------------------------------------------
