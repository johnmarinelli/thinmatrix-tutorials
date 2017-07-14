#ifndef MousePicker_hpp
#define MousePicker_hpp

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Terrain.hpp"

const int MAX_RECURSION = 600;
const float INITIAL_RAY_LENGTH = 200.0f;

class MousePicker {
public:
  glm::vec3 mCurrentRay;
  glm::mat4 mProjectionMatrix;
  glm::mat4 mViewMatrix;
  
  Camera* mCameraHdl;
  Terrain* mTerrainHdl;
  
  MousePicker();
  
  void init(const glm::mat4& projectionMatrix, Camera& cam, Terrain& terrain);
  
  void update(const glm::mat4& viewMatrix);
  
  glm::vec3 calculateMouseRay(float mX, float mY);
  
  glm::vec2 getNormalizedDeviceCoords(float mX, float mY);
  glm::vec4 clipToEye(const glm::vec4& clipCoords);
  glm::vec3 eyeToWorld(const glm::vec4& eyeCoords);
  
  glm::vec3 binarySearch(int count, float start, float finish, const glm::vec3& ray);
  glm::vec3 getPointOnRay(const glm::vec3& ray, float dis);
  bool intersectionInRange(float start, float end, const glm::vec3& ray);
  bool isUnderground(const glm::vec3& testPoint);
  
};


#endif
