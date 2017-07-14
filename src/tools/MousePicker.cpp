#include "MousePicker.hpp"
#include "Utilities.h"

MousePicker::MousePicker() {
}

void MousePicker::init(const glm::mat4& projectionMatrix, Camera& cam, Terrain& terrain) {
  mProjectionMatrix = projectionMatrix;
  mCameraHdl = &cam;
  mTerrainHdl = &terrain;
}

void MousePicker::update(const glm::mat4& viewMatrix) {
  mViewMatrix = viewMatrix;
}

glm::vec3 MousePicker::calculateMouseRay(float mX, float mY) {  
  auto ndcc = getNormalizedDeviceCoords(mX, mY);
  glm::vec4 clipCoords = glm::vec4{ndcc, -1.0f, 1.0f};
  
  auto eyeCoords = clipToEye(clipCoords);
  auto worldCoords = eyeToWorld(eyeCoords);
  
  mCurrentRay = worldCoords;
  
  std::cout << "Mouse Ray: " << mCurrentRay.x << ", " << mCurrentRay.y << ", " << mCurrentRay.z << '\n';
  
  if (intersectionInRange(0.0f, INITIAL_RAY_LENGTH, mCurrentRay)) {
    glm::vec3 intersection = binarySearch(0, 0.0f, INITIAL_RAY_LENGTH, mCurrentRay);
    
    
    std::cout << "Intersection: " << intersection.x << ", " << intersection.y << ", " << intersection.z << '\n';
  }
  
  return mCurrentRay;
}

glm::vec2 MousePicker::getNormalizedDeviceCoords(float mX, float mY) {
  float x = ((2.0f * mX) / VIEWPORT_WIDTH) - 1.0f;
  float y = 1.0f - ((2.0f * mY) / VIEWPORT_HEIGHT);
  return glm::vec2{x, y};
}

glm::vec4 MousePicker::clipToEye(const glm::vec4& clipCoords) {
  auto invProj = glm::inverse(mProjectionMatrix);
  glm::vec4 eyeCoords = invProj * clipCoords;
  // don't care about w component.  set z component to point inside the screen.
  return glm::vec4{eyeCoords.x, eyeCoords.y, -1.0f, 0.0f};
}

glm::vec3 MousePicker::eyeToWorld(const glm::vec4& eyeCoords) {
  glm::mat4 invView = glm::inverse(mViewMatrix);
  glm::vec4 ray = invView * eyeCoords;
  return glm::normalize(glm::vec3{ray.x, ray.y, ray.z});
}

glm::vec3 MousePicker::binarySearch(int count, float start, float finish, const glm::vec3& ray) {
  float half = start + ((finish - start) / 2.0f);
  if (count >= MAX_RECURSION) {
    glm::vec3 endPoint = getPointOnRay(ray, half);
    return endPoint;
  }
  if (intersectionInRange(start, start + half, ray)) {
    return binarySearch(count + 1, start, half, ray);
  }
  else {
    return binarySearch(count + 1, half, finish, ray);
  }
}

// scales ray by dis.
glm::vec3 MousePicker::getPointOnRay(const glm::vec3& ray, float dis) {
  glm::vec3 camPos = mCameraHdl->mPosition;
  glm::vec3 start = camPos;
  glm::vec3 scaledRay = ray * dis;
  return start + scaledRay;
}

// checks to see if there's an intersection between
// ray and terrain.
bool MousePicker::intersectionInRange(float start, float end, const glm::vec3& ray) {
  glm::vec3 startPoint = getPointOnRay(ray, start);
  glm::vec3 endPoint = getPointOnRay(ray, end);
  
  return !isUnderground(startPoint) && isUnderground(endPoint);
}

bool MousePicker::isUnderground(const glm::vec3& testPoint) {
  float height = 0.0f;
  height = mTerrainHdl->getHeightAtCoord(testPoint.x, testPoint.z);
  
  return testPoint.y < height;
}
