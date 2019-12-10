#include "Model.h"

//----------------------------------------------
//See Model.h for basic documentation.
//----------------------------------------------

int screenWidth_model = 0;
int screenHeight_model = 0;

Entity::Entity() {
    hit = NULL; //set the pointer functions to null at creation to avoid calling a function that hasn't been defined yet.
    update = NULL;
    shoot = NULL;

    timer = timer1 = timer2 = lives = 0;
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
Entity *player_pointer;

//vector is passed from controller.cpp to spawn function. spawn creates entity and passes to this with vec.
void finalize_entity(Entity target, std::vector<Entity> *ent_list) {
  ent_list->push_back(target);
}

//Update function for the player.
//Move the position(hitbox) by velocity * scale. (coming soon)
void player_update() {

  player_pointer->rect.x += player_pointer->xVel;
  if(player_pointer->rect.x + player_pointer->rect.width > screenWidth_model || player_pointer->rect.x < 0) {
      player_pointer->rect.x -= player_pointer->xVel;
  }

  player_pointer->rect.y += player_pointer->yVel;
  if(player_pointer->rect.y + player_pointer->rect.height > screenHeight_model || player_pointer->rect.y < 0) {
      player_pointer->rect.y -= player_pointer->yVel;
  }

  if(player_pointer->health<=0) player_pointer->status = ENTITY_DESTROY;

  //Set the frame depending on direction of movement.
  if(player_pointer->xVel < 0) player_pointer->frame = 0;
  if(player_pointer->xVel == 0) player_pointer->frame = 1;
  if(player_pointer->xVel > 0) player_pointer->frame = 2;
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
  if(self->rect.y + self->rect.height < 0 || self->rect.y > screenHeight_model)
     {
        self->status = ENTITY_REMOVE;
     }
  if(self->health <= 0) {
    self->status = ENTITY_DESTROY;
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
    new_entity.yVel = -10;
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

void player_hit(){
    self->health -= 1;
}

void enemy_bullet_hit() {
  self->health -= 1;
  self = other;
  if(other->hit != NULL) other->hit();
}

void enemy_hit() {
  if(other->className == BASIC_BULLET) {
    self->health -= 1;
    self = other;
    other->hit();
  }
  if(other->className == PLAYER_PLAYER) {
    self->points = 0;
    self->health = 0;
    self = other;
    other->hit();
  }
}

void enemy_shoot(std::vector<Entity> *ent_list) {
  Entity new_entity;

  //assign non type specific vars.
  new_entity.update = &bullet_basic_update;
  new_entity.yVel = 7;
  new_entity.health = 1;
  new_entity.className = 0;
  new_entity.textureName = 2;
  new_entity.hit = &enemy_bullet_hit;
  new_entity.timer = 0;
  new_entity.timer1 = 0;
  new_entity.rect.width = 10*self->xScale; //scale is passed from controller.cpp
  new_entity.rect.height = 10*self->yScale;
  new_entity.rect.x = self->rect.x + self->rect.width / 2 - new_entity.rect.width/2;
  new_entity.rect.y = self->rect.y + self->rect.height;
  new_entity.xVel = 0; //don't forget to set.
  new_entity.status = ENTITY_KEEP; //entities should start with this.
  new_entity.frame = 0;
  new_entity.xScale = self->xScale;
  new_entity.yScale = self->yScale;

  finalize_entity(new_entity, ent_list);
}

void basic_enemy_update() {
  if(self->rect.y + self->rect.height < 0) {
    self->rect.y += 2*self->yScale;
  }
  else {
    self->rect.x += self->xVel*self->xScale;
    self->rect.y += self->yVel*self->yScale;
  }

  self->timer++;

  //----------------------------------------------
  //goal is to achieve a rotating effect with the sprite sheet.
  //frames go up when moving right, down when left.
  //----------------------------------------------
  if(self->timer > 10) { //every tenth of a second adjust the frame.
    self->timer = 0; //reset timer.

    if(self->xVel < 0) { //check wich direction the entity is moving.
      self->frame--; //if moving left frames go down.
    }
    else if(self->xVel > 0) { //check direction.
      self->frame++; //if moving right frames go up.
    }
    else { //eh, not thinking i guess. could have just done if, else on the first to not if, else if.
      self->frame++;
    }

    if(self->frame > 2) { //sprite sheet only has three frames, 0,1,2
      self->frame = 0; //if greater than 2 reset to zero.
    }
    if(self->frame < 0) { //
      self->frame = 2;// if less than 0 reset to 2.
    }
  }

  if(self->rect.y > screenHeight_model ||
     self->rect.x + self->rect.width < 0 ||
     self->rect.x > screenWidth_model)
     {
        self->status = ENTITY_REMOVE;
     }

  if(self->health <= 0) {
    self->status = ENTITY_DESTROY;
  }
}

void adv_enemy_update() {
  if(self->rect.y + self->rect.height < 0) {
    self->rect.y += 2*self->yScale;
  }
  else {
    self->rect.x += self->xVel*self->xScale;
    self->rect.y += self->yVel*self->yScale;
  }

  self->timer++;
  self->timer1++;

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
      self->frame = 2;
    }
  }

  if(self->timer1 > 60) {
    self->timer1 = 0;
    if(self->xVel < 0) self->xVel = 5;
    else if(self->xVel > 0) self->xVel = -5;
    else if(self->xVel == 0) {
      if(self->rect.x + self->rect.width/2 < screenWidth_model/2)
        self->xVel = 5;
      else self->xVel = -5;
    }
  }

  if(self->rect.y + self->rect.height < 0) {
    self->xVel = 0;
    self->timer1 = 60;
  }

  if(self->rect.y > screenHeight_model ||
     self->rect.x + self->rect.width < 0 ||
     self->rect.x > screenWidth_model)
     {
        self->status = ENTITY_REMOVE;
     }

  if(self->health <= 0) {
    self->status = ENTITY_DESTROY;
  }
}

void tracker_enemy_update() {
  if(self->health > 0) {
    self->status = ENTITY_KEEP;
  }

  if(self->rect.y + self->rect.height < 0) {
    self->rect.y += 2*self->yScale;
  }
  else {
    self->rect.x += self->xVel*self->xScale;
    self->rect.y += self->yVel*self->yScale;
  }

  if(self->rect.x + self->rect.width / 2 < player_pointer->rect.x + player_pointer->rect.width && self->rect.x + self->rect.width/2 > player_pointer->rect.x && self->timer2 >= 60) {
    self->status = ENTITY_SHOOT;
  }

  self->timer++;
  self->timer1++;
  self->timer2++;

  if(self->timer2 > 60) self->timer2 = 0;

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
      self->frame = 2;
    }
  }

  if(self->rect.x + (self->rect.width/2) > player_pointer->rect.x + (player_pointer->rect.width/2)) self->xVel = -2;
  else if(self->rect.x + (self->rect.width/2) < player_pointer->rect.x + (player_pointer->rect.width/2)) self->xVel = 2;
  else self->xVel = 0;

  if(self->rect.y < 0) {
    self->xVel = 0;
  }

  if(self->rect.y > screenHeight_model ||
     self->rect.x + self->rect.width < 0 ||
     self->rect.x > screenWidth_model)
     {
        self->status = ENTITY_REMOVE;
     }

  if(self->health <= 0) {
    self->status = ENTITY_DESTROY;
  }
}

void bomber_enemy_update() {
  if(self->rect.y + self->rect.height < 0) {
    self->rect.y += 2*self->yScale;
  }
  else {
    self->rect.x += self->xVel*self->xScale;
    self->rect.y += self->yVel*self->yScale;
  }

  self->timer++;

  //----------------------------------------------
  //goal is to achieve a rotating effect with the sprite sheet.
  //frames go up when moving right, down when left.
  //----------------------------------------------
  if(self->timer > 10) { //every tenth of a second adjust the frame.
    self->timer = 0; //reset timer.

    if(self->xVel < 0) { //check wich direction the entity is moving.
      self->frame--; //if moving left frames go down.
    }
    else if(self->xVel > 0) { //check direction.
      self->frame++; //if moving right frames go up.
    }
    else { //eh, not thinking i guess. could have just done if, else on the first to not if, else if.
      self->frame++;
    }

    if(self->frame > 2) { //sprite sheet only has three frames, 0,1,2
      self->frame = 0; //if greater than 2 reset to zero.
    }
    if(self->frame < 0) { //
      self->frame = 2;// if less than 0 reset to 2.
    }
  }
  if(self->rect.y + (self->rect.height/2) >= player_pointer->rect.y + (player_pointer->rect.height/2) && self->yVel != 0){
      self->yVel = 0;
      if (self->rect.x > player_pointer->rect.x) self->xVel = -6;
      else self->xVel = 6;
  }

  if(self->rect.y > screenHeight_model ||
     self->rect.x + self->rect.width < 0 ||
     self->rect.x > screenWidth_model)
     {
        self->status = ENTITY_REMOVE;
     }

  if(self->health <= 0) {
    self->status = ENTITY_DESTROY;
  }
}

void fighter_update() {
  if(self->health > 0) {
    self->status = ENTITY_KEEP;
  }

  if(self->rect.y + self->rect.height < 0) {
    self->rect.y += 2*self->yScale;
  }
  else {
    self->rect.x += self->xVel*self->xScale;
    self->rect.y += self->yVel*self->yScale;
  }

  if(self->rect.y > 0 + 100*self->yScale) {
      self->yVel = 0;
  }

  self->timer++;
  if(self->timer > 30) {
      self->timer1++;
      self->timer = 0;
      self->status = ENTITY_SHOOT;
      if(self->timer1 >= 3) {
          self->timer1 = 0;
          self->timer = -60;
      }
  }

  if(self->xVel < 0) self->frame = 0;
  else if(self->xVel > 0) self->frame = 2;
  else self->frame = 1;

  if(self->rect.x <= 0 + 200*self->xScale) {
      self->xVel = 5;
  }
  if(self->rect.x + self->rect.width >= screenWidth_model - 200*self->xScale) {
      self->xVel = -5;
  }

  if(self->health <= 0) {
    self->status = ENTITY_DESTROY;
  }
}

void spawn_enemy(int x, int y, float xScale, float yScale, int type, std::vector<Entity> *ent_list) {
  Entity new_entity; //create a new entity.

  //----------------------------------------------
  //check the type and assign entity specific
  //----------------------------------------------
  if(type == 0) {
    new_entity.update = &basic_enemy_update;
    new_entity.health = 1;
    new_entity.xVel = 0;
    new_entity.yVel = 2;
    new_entity.rect.width = 150*xScale;
    new_entity.rect.height = 150*yScale;
    new_entity.textureName = 6;
    new_entity.points = 26;
  }

  else if(type == 1) {
    new_entity.update = &adv_enemy_update;
    new_entity.health = 2;
    if(x < screenWidth_model) {
      new_entity.xVel = 5;
    }
    else new_entity.xVel = -5;

    new_entity.yVel = 1;
    new_entity.rect.width = 150*xScale;
    new_entity.rect.height = 150*yScale;
    new_entity.textureName = 7;
    new_entity.points = 33;
  }
  else if(type == 2) {
    new_entity.update = &tracker_enemy_update;
    new_entity.shoot = &enemy_shoot;
    new_entity.health = 4;
    new_entity.xVel = 0;
    new_entity.yVel = 1;
    new_entity.rect.width = 150*xScale;
    new_entity.rect.height = 150*yScale;
    new_entity.textureName = 8;
    new_entity.points = 40;
  }
  else if(type == 3) {
    new_entity.update = &bomber_enemy_update;
    new_entity.health = 1;
    new_entity.xVel = 0;
    new_entity.yVel = 2;
    new_entity.rect.width = 150*xScale;
    new_entity.rect.height = 150*yScale;
    new_entity.textureName = 6;
    new_entity.points = 29;
  }
  else if(type == 4) {
    new_entity.update = &fighter_update;
    new_entity.health = 20;
    new_entity.xVel = -5;
    new_entity.yVel = 2;
    new_entity.rect.width = 146*xScale;
    new_entity.rect.height = 140*yScale;
    new_entity.textureName = 10;
    new_entity.points = 1500;
  }

  new_entity.hit = &enemy_hit;
  new_entity.shoot = &enemy_shoot;
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.frame = 0;
  new_entity.timer = 0;
  new_entity.timer1 = 0;
  new_entity.status = ENTITY_KEEP;
  new_entity.xScale = xScale;
  new_entity.yScale = yScale;
  new_entity.className = type;

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

void beam_burst_update() {
  self->timer++;
  if(self->timer > 5) {
    self->frame++;
    self->timer = 0;
  }
  if(self->frame > 3) {
    self->status = ENTITY_REMOVE;
  }
}

void spawn_explosion(int x, int y, int type, std::vector<Entity> *ent_list) {
  Entity new_entity;
  new_entity.rect.x = x;
  new_entity.rect.y = y;
  new_entity.frame = 0;
  new_entity.textureName = 0;
  if(type == 0) {
    new_entity.update = &explosion_update;
    new_entity.textureName = 0;
  }
  if(type == 1) {
    new_entity.update = &beam_burst_update;
    new_entity.textureName = 9;
  }
  new_entity.status = ENTITY_KEEP;
  new_entity.timer = 0;
  new_entity.timer1 = 0;

  finalize_entity(new_entity, ent_list);
}
