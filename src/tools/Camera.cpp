#include "Camera.hpp"


Camera::Camera() :
  mPosition(glm::vec3{0.0f, 0.0f, 0.0f}),
  mCameraStep(1.0f),
  mDistanceFromPlayer(50.0f),
  mAngleAroundPlayer(0.0f),
  mPitch(25.0f) {
  mViewMatrix = createViewMatrix();
}

void Camera::zoom(float yOffset) {
  calculateZoom(yOffset);
}

void Camera::rotate(float xDelta) {
  mAngleAroundPlayer += xDelta * 0.3f;
}

void Camera::moveVertical(float yDelta) {
  mPitch -= yDelta * 0.1f;
}

void Camera::update(double dt) {
  auto hDistance = calculateHorizontalDistance();
  auto vDistance = calculateVerticalDistance();
  
  calculateCameraPosition(hDistance, vDistance);
  mYaw = 180.0f - (mPlayerHdl->mRotationAngle + mAngleAroundPlayer);
}

glm::mat4 Camera::createViewMatrix() const {
  glm::mat4 view{1.0f};
  view = glm::rotate(view, toRadians(mPitch), glm::vec3{1.0f, 0.0f, 0.0f});
  view = glm::rotate(view, toRadians(mYaw), glm::vec3{0.0f, 1.0f, 0.0f});
  
  glm::vec3 posInv = mPosition * -1.0f;
  view = glm::translate(view, posInv);
  return view;
}

void Camera::calculateCameraPosition(float horizDistance, float vertDistance) {
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

float Camera::calculateHorizontalDistance() {
  return mDistanceFromPlayer * std::cos(toRadians(mPitch));
}

float Camera::calculateVerticalDistance() {
  return mDistanceFromPlayer * std::sin(toRadians(mPitch));
}

inline void Camera::calculateZoom(float yOffset) {
  float zoomLevel = yOffset * 0.1f;
  mDistanceFromPlayer -= zoomLevel;
}

void Camera::calculatePitch(float cursorX, float cursorY) {
  float pitchDelta = cursorY * 0.1f;
  mPitch -= pitchDelta;
}

void Camera::calculateAngleAroundPlayer(float cursorX, float cursorY) {
  float angleDelta = cursorX * 0.3f;
  mAngleAroundPlayer -= angleDelta;
}
