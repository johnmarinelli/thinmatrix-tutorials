#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Player.hpp"
#include "Utilities.h"

enum class MovementDirection {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Camera {
public:
  glm::vec3 mPosition;
  glm::mat4 mViewMatrix;
  
  float mPitch, mYaw, mRoll;
  float mCameraStep;
  
  std::shared_ptr<Player> mPlayerHdl;
  float mDistanceFromPlayer;
  float mAngleAroundPlayer;
  
  Camera() :
    mPosition(glm::vec3{0.0f, 0.0f, 0.0f}),
    mCameraStep(1.0f),
    mDistanceFromPlayer(50.0f),
    mAngleAroundPlayer(0.0f),
    mPitch(25.0f) {
    mViewMatrix = createViewMatrix();
  }
  
  void zoom(float yOffset) {
    calculateZoom(yOffset);
  }
  
  void rotate(float xDelta) {
    mAngleAroundPlayer += xDelta * 0.3f;
  }
  
  void moveVertical(float yDelta) {
    mPitch -= yDelta * 0.1f;
  }
  
  void update(double dt) {
    auto hDistance = calculateHorizontalDistance();
    auto vDistance = calculateVerticalDistance();
    
    calculateCameraPosition(hDistance, vDistance);
    mYaw = 180.0f - (mPlayerHdl->mRotationAngle + mAngleAroundPlayer);
  }
  
  glm::mat4 createViewMatrix() const {
    glm::mat4 view{1.0f};
    view = glm::rotate(view, toRadians(mPitch), glm::vec3{1.0f, 0.0f, 0.0f});
    view = glm::rotate(view, toRadians(mYaw), glm::vec3{0.0f, 1.0f, 0.0f});
    
    glm::vec3 posInv = mPosition * -1.0f;
    view = glm::translate(view, posInv);
    return view;
  }
  
  void calculateCameraPosition(float horizDistance, float vertDistance) {
    float theta = toRadians(mPlayerHdl->mRotationAngle + mAngleAroundPlayer);
    auto offsetX = horizDistance * std::sin(theta);
    auto offsetZ = horizDistance * std::cos(theta);
    
    mPosition.x = mPlayerHdl->mPosition.x - offsetX;
    mPosition.y = mPlayerHdl->mPosition.y + vertDistance + 5.0; // 25.0 is so camera isn't over the player's feet
    
    if (mPosition.y < 1.0) {
      mPosition.y = 1.0;
    }
    if (mPosition.y > 100.0) {
      mPosition.y = 100.0;
    }
    
    mPosition.z = mPlayerHdl->mPosition.z - offsetZ;
  
  }
  
  float calculateHorizontalDistance() {
    return mDistanceFromPlayer * std::cos(toRadians(mPitch));
  }
  
  float calculateVerticalDistance() {
    return mDistanceFromPlayer * std::sin(toRadians(mPitch));
  }
  
  inline void calculateZoom(float yOffset) {
    float zoomLevel = yOffset * 0.1f;
    mDistanceFromPlayer -= zoomLevel;
  }
  
  void calculatePitch(float cursorX, float cursorY) {
    float pitchDelta = cursorY * 0.1f;
    mPitch -= pitchDelta;
  }
  
  void calculateAngleAroundPlayer(float cursorX, float cursorY) {
    float angleDelta = cursorX * 0.3f;
    mAngleAroundPlayer -= angleDelta;
  }
};

#endif
