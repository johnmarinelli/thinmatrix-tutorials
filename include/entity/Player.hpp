#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "Terrain.hpp"
#include "Entity.hpp"
#include "Utilities.h"

enum class PlayerMovementDirection {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  JUMP
};

class Player : public Entity {
public:
  Player();
  Player(const TexturedModel& model, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl);
  
  void update(double dt, const Terrain& terrain);
  
  void handleInput(PlayerMovementDirection dir, int state);

  const float RUN_SPEED = 50.0f;
  const float TURN_SPEED = 160.0f;
  const float GRAVITY = GLOBAL_GRAVITY;
  const float JUMP_POWER = 30.0;
  const float TERRAIN_HEIGHT = 0.0f;
  
  float mCurrentSpeed;
  float mCurrentTurnSpeed;
  float mUpwardsSpeed;
  
  bool mIsInAir;
  
};

#endif
