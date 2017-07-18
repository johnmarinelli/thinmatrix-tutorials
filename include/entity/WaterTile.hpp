#ifndef WaterTile_hpp
#define WaterTile_hpp

#include <glm/glm.hpp>

class WaterTile {
public:
  const float TILE_SIZE = 60.0f;
  
  float mHeight;
  float mX, mZ;
  
  WaterTile(float cX, float cZ, float h);
};

#endif
