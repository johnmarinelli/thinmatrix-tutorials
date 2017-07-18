#ifndef STB_IMAGE_HPP
#define STB_IMAGE_HPP

#include "stb_image.h"
#include <glm/glm.hpp>

class StbImage {
public:
  StbImage(const char* filepath, int numComponents = 0) :
    mData(stbi_load(filepath, &mWidth, &mHeight, &mNumComponents, numComponents)) {
  }  
  
  glm::ivec3 getRGB(int x, int y) const {
    int index = (y * mWidth + x) * mNumComponents;
    int r = mData[index];
    int g = mData[index + 1];
    int b = mData[index + 2];
    return glm::ivec3{r, g, b};
  }
  
  int mWidth, mHeight;
  int mNumComponents;
  unsigned char* mData;
};

#endif
