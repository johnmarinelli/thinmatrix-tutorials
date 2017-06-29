#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <iostream>
#include <GLFW/glfw3.h>
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
  
  const float RUN_SPEED = 20.0f;
  const float TURN_SPEED = 160.0f;
  const float GRAVITY = -50.0f;
  const float JUMP_POWER = 30.0;
  const float TERRAIN_HEIGHT = 0.0f;
  
  float mCurrentSpeed;
  float mCurrentTurnSpeed;
  float mUpwardsSpeed;
  
  bool mIsInAir;
  
  Player() :
    Entity() {    
  }
  
  Player(const TexturedModel& model, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) :
    Entity(model, pos, rot, scl),
    mCurrentSpeed(0.0f),
    mCurrentTurnSpeed(0.0f),
    mUpwardsSpeed(0.0f),
    mIsInAir(false) {
  }
  
  void update(double dt) {
    auto dtSeconds = dt * 1000.0;
    
    mUpwardsSpeed += GRAVITY * dtSeconds;
    mRotationAngle += mCurrentTurnSpeed * dtSeconds;
    float dis = mCurrentSpeed * dtSeconds;
    
    auto dx = dis * std::sin(toRadians(mRotationAngle));
    auto dy = dtSeconds * mUpwardsSpeed;
    auto dz = dis * std::cos(toRadians(mRotationAngle));
    
    mPosition += glm::vec3{dx, dy, dz};
    
    if (mPosition.y < TERRAIN_HEIGHT) {
      mUpwardsSpeed = 0.0f;
      mPosition.y = TERRAIN_HEIGHT;
      mIsInAir = false;
    }

    std::cout << "Player position: " << mPosition.x << ", " << mPosition.y << ", " << mPosition.z << '\n';
  }
  
  void handleInput(PlayerMovementDirection dir, int state) {
    if (PlayerMovementDirection::UP == dir) {
      if (state == GLFW_PRESS) mCurrentSpeed += RUN_SPEED;
      if (state == GLFW_RELEASE) mCurrentSpeed -= RUN_SPEED;
    }
    if (PlayerMovementDirection::DOWN == dir) {
      if (state == GLFW_PRESS) mCurrentSpeed -= RUN_SPEED;
      if (state == GLFW_RELEASE) mCurrentSpeed += RUN_SPEED;
    }
    if (PlayerMovementDirection::LEFT == dir) {
      if (state == GLFW_PRESS) mCurrentTurnSpeed += TURN_SPEED;
      if (state == GLFW_RELEASE) mCurrentTurnSpeed -= TURN_SPEED;
    }
    if (PlayerMovementDirection::RIGHT == dir) {
      if (state == GLFW_PRESS) mCurrentTurnSpeed -= TURN_SPEED;
      if (state == GLFW_RELEASE) mCurrentTurnSpeed += TURN_SPEED;
    }
    if (PlayerMovementDirection::JUMP == dir) {
      if (state == GLFW_PRESS && !mIsInAir) {
        mUpwardsSpeed = JUMP_POWER;
        mIsInAir = true;
      }
    }
  }
  
};

#endif
