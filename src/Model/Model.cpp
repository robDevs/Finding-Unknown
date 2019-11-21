#include "Model.h"

//----------------------------------------------
//See Model.h for basic documentation.
//----------------------------------------------

int screenWidth_model = 0;
int screenHeight_model = 0;

Entity::Entity() {
    hit = NULL; //set the pointer functions to null at creation to avoid calling a function that hasn't been defined yet.
    update = NULL;
}

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
Rectangle Entity::getRect() {
    return rect;
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
// Entity pointer functions go here.
//---------------------------------------------

Entity *self; //like this->
Entity *other; //EG: bullet hits enemy. bullet == other. enemy = this.
Entity *player;

//vector is passed from controller.cpp to spawn function. spawn creates entity and passes to this with vec.
void finalize_entity(Entity target, std::vector<Entity> *ent_list) {
  ent_list->push_back(target);
}

//Update function for the player.
//Move the position(hitbox) by velocity * scale. (coming soon)
void player_update() {
  self->rect.x += self->xVel;
  self->rect.y += self->yVel;

  //Set the frame depending on direction of movement.
  if(self->xVel < 0) self->frame = 0;
  if(self->xVel == 0) self->frame = 1;
  if(self->xVel > 0) self->frame = 2;
}
//Only one player so manually created in controller.cpp. no spawn function.

//Update the bullet.
void bullet_basic_update() {
  //Move the bullet.
  self->rect.x += self->xVel*self->yScale;
  self->rect.y += self->yVel*self->yScale;

  //Increase timer.
  self->timer++;

  //set frame depending on timer.
  //reest timer.
  if(self->timer > 15) {
    self->timer = 0;
    self->frame++;
  }

  if(self->frame > 1) {
    self->frame = 0;
  }

  //entity should be removed when off screen or when health is 0
  //status is checked and handled each frame by controller.cpp
  if(self->rect.y + self->rect.height < 0 || self->health <= 0)
     {
        self->status = ENTITY_REMOVE;
     }
}
//if bullet is hit, lower it's health.
//Bullets should only have one health though.
void basic_bullet_hit() {
  self->health -= 1;
}
//spawn a bullet at given (x,y)
void spawn_bullet(int x, int y, int type, float xScale, float yScale, std::vector<Entity> *ent_list){
  Entity new_entity; //create a new ent.

  if(type == BASIC_BULLET) { //check it's type and assign type specific vars.
    new_entity.update = &bullet_basic_update;
    new_entity.yVel = -7;
    new_entity.health = 1;
    new_entity.className = type;
    new_entity.textureName = 2;
    new_entity.hit = &basic_bullet_hit;
    new_entity.timer = 0;
  }
  //assign non type specific vars.
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.xVel = 0; //don't forget to set.
  new_entity.rect.width = 10*xScale; //scale is passed from controller.cpp
  new_entity.rect.height = 10*yScale;
  new_entity.status = ENTITY_KEEP; //entities should start with this.
  new_entity.frame = 0;
  new_entity.xScale = xScale;
  new_entity.yScale = yScale;

  finalize_entity(new_entity, ent_list);
}

void test_enemy_update() {
  self->rect.x += self->xVel*self->xScale;
  self->rect.y += self->yVel*self->yScale;

  self->timer++;

  if(self->timer > 10) {
    self->timer = 0;

    if(self->xVel < 0) {
      self->frame--;
    }
    else if(self->xVel > 0) {
      self->frame++;
    }
    else {
      self->frame++;
    }

    if(self->frame > 2) {
      self->frame = 0;
    }
    if(self->frame < 0) {
      self->frame = 0;
    }
  }

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
void spawn_test_enemy(int x, int y, float xScale, float yScale, std::vector<Entity> *ent_list) {
  Entity new_entity;

  new_entity.update = &test_enemy_update;
  new_entity.hit = &test_enemy_hit;
  new_entity.health = 1;
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.yVel = 5;
  new_entity.xVel = 0;
  new_entity.rect.width = 150*xScale;
  new_entity.rect.height = 150*yScale;
  new_entity.frame = 0;
  new_entity.timer = 0;
  new_entity.textureName = 6;
  new_entity.status = ENTITY_KEEP;
  new_entity.points = 26;
  new_entity.xScale = xScale;
  new_entity.yScale = yScale;

  finalize_entity(new_entity, ent_list);
}


void burner_update() {
  self->timer++;
  if(self->timer > 20) {
    self->frame++;
    self->timer = 0;
  }
  if(self->frame > 3) {
    self->frame = 0;
  }
}

void explosion_update() {
  self->timer++;
  if(self->timer > 5) {
    self->frame++;
    self->timer = 0;
  }
  if(self->frame > 11) {
    self->status = ENTITY_REMOVE;
  }
}
void spawn_explosion(int x, int y, std::vector<Entity> *ent_list) {
  Entity new_entity;
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.frame = 0;
  new_entity.textureName = 0;
  new_entity.update = &explosion_update;
  new_entity.status = ENTITY_KEEP;

  finalize_entity(new_entity, ent_list);
}
