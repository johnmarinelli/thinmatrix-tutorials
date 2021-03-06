#include "Player.hpp"


Player::Player() :
  Entity() {
  mRotationAngle = 0.0f;
}

Player::Player(const TexturedModel& model, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) :
  Entity(model, 0, pos, rot, scl),
  mCurrentSpeed(0.0f),
  mCurrentTurnSpeed(0.0f),
  mUpwardsSpeed(0.0f),
  mIsInAir(false) {
  mRotationAngle = 0.0f;
}

void Player::update(double dt, const Terrain& terrain) {
  auto dtSeconds = dt * 1000.0;
  
  mUpwardsSpeed += GRAVITY * dtSeconds;
  mRotationAngle += mCurrentTurnSpeed * dtSeconds;
  float dis = mCurrentSpeed * dtSeconds;
  
  auto dx = dis * std::sin(toRadians(mRotationAngle));
  auto dy = dtSeconds * mUpwardsSpeed;
  auto dz = dis * std::cos(toRadians(mRotationAngle));
  
  mPosition += glm::vec3{dx, dy, dz};
  
  auto terrainHeight = terrain.getHeightAtCoord(mPosition.x, mPosition.z);
  
  if (mPosition.y < terrainHeight) {
    mUpwardsSpeed = 0.0f;
    mPosition.y = terrainHeight;
    mIsInAir = false;
  }
  std::cout << "Player position: " << mPosition.x << ", " << mPosition.y << ", " << mPosition.z << '\n';
}

void Player::handleInput(PlayerMovementDirection dir, int state) {
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
