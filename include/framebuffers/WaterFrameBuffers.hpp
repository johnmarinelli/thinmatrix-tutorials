#ifndef WaterFrameBuffers_hpp
#define WaterFrameBuffers_hpp

#include <OpenGL/gl3.h>

class WaterFrameBuffers {
public:
  const int REFLECTION_WIDTH = 320;
  const int REFLECTION_HEIGHT = 180;
  const int REFRACTION_WIDTH = 1280;
  const int REFRACTION_HEIGHT = 720;
  
  WaterFrameBuffers();
  
  void initializeReflectionFrameBuffer();
  void initializeRefractionFrameBuffer();
  
  void bindReflectionFrameBuffer();
  void bindRefractionFrameBuffer();
  
  void unbindCurrentFrameBuffer();
  void bindFrameBuffer(int fb, int w, int h);
  GLuint createFrameBuffer();
  
  void cleanUp();
  
  GLuint createTextureAttachment(int width, int height);
  GLuint createDepthTextureAttachment(int width, int height);
  GLuint createDepthBufferAttachment(int width, int height);
  
  GLuint mReflectionFrameBufferID;
  GLuint mReflectionTextureID;
  GLuint mReflectionDepthBufferID;
  
  GLuint mRefractionFrameBufferID;
  GLuint mRefractionTextureID;
  GLuint mRefractionDepthBufferID;
  
};

#endif
