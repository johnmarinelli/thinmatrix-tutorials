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

  Camera();
  
  void zoom(float yOffset);
  
  void rotate(float xDelta);
  
  void moveVertical(float yDelta);
  
  void update(double dt);
  
  glm::mat4 createViewMatrix() const;
  
  void calculateCameraPosition(float horizDistance, float vertDistance);
  
  float calculateHorizontalDistance();
  
  float calculateVerticalDistance();
  
  inline void calculateZoom(float yOffset);
  void calculatePitch(float cursorX, float cursorY);
  void calculateAngleAroundPlayer(float cursorX, float cursorY);
  
  glm::vec3 mPosition;
  glm::mat4 mViewMatrix;
  
  float mPitch, mYaw, mRoll;
  float mCameraStep;
  
  std::shared_ptr<Player> mPlayerHdl;
  float mDistanceFromPlayer;
  float mAngleAroundPlayer;
};

#endif
