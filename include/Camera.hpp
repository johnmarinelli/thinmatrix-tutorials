#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
  
  Camera() :
    mPosition(glm::vec3{0.0f, 0.0f, 0.0f}),
    mCameraStep(1.0f) {
    mViewMatrix = createViewMatrix();
  }
  
  void move(MovementDirection dir) {
    if (MovementDirection::UP == dir) {
      mPosition.z -= mCameraStep;
    }
    else if (MovementDirection::DOWN == dir) {
      mPosition.z += mCameraStep;
    }
    else if (MovementDirection::LEFT == dir) {
      mPosition.x -= mCameraStep;
    }
    else if (MovementDirection::RIGHT == dir) {
      mPosition.x += mCameraStep;
    }
  }
  
  glm::mat4 createViewMatrix() {
    glm::mat4 view{1.0f};
    view = glm::rotate(view, toRadians(mPitch), glm::vec3{1.0f, 0.0f, 0.0f});
    view = glm::rotate(view, toRadians(mYaw), glm::vec3{0.0f, 1.0f, 0.0f});
    
    glm::vec3 posInv = mPosition * -1.0f;
    view = glm::translate(view, posInv);
    return view;
  }
};

#endif
