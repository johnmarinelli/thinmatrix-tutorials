#include "WaterFrameBuffers.hpp"
#include "Utilities.h"

WaterFrameBuffers::WaterFrameBuffers() {
  initializeReflectionFrameBuffer();
  initializeRefractionFrameBuffer();
}

void WaterFrameBuffers::initializeReflectionFrameBuffer() {
  mReflectionFrameBufferID = createFrameBuffer();
  mReflectionTextureID = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
  mReflectionDepthBufferID = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
  unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::initializeRefractionFrameBuffer() {
  mRefractionFrameBufferID = createFrameBuffer();
  mRefractionTextureID = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
  mRefractionDepthBufferID = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffers::bindReflectionFrameBuffer() {
  bindFrameBuffer(mReflectionFrameBufferID, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFrameBuffers::bindRefractionFrameBuffer() {
  bindFrameBuffer(mRefractionFrameBufferID, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffers::unbindCurrentFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
}

void WaterFrameBuffers::bindFrameBuffer(int fb, int w, int h) {
  // unbind last texture
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glViewport(0, 0, w, h);
}

GLuint WaterFrameBuffers::createFrameBuffer() {
  GLuint fb;
  glGenFramebuffers(1, &fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  return fb;
}

void WaterFrameBuffers::cleanUp() {
  glDeleteFramebuffers(1, &mReflectionFrameBufferID);
  glDeleteTextures(1, &mReflectionTextureID);
  glDeleteRenderbuffers(1, &mReflectionDepthBufferID);
  glDeleteFramebuffers(1, &mRefractionFrameBufferID);
  glDeleteTextures(1, &mRefractionTextureID);
  glDeleteTextures(1, &mRefractionDepthBufferID);
}

GLuint WaterFrameBuffers::createTextureAttachment(int width, int height) {
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*) nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texID, 0);
  return texID;
}

GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height) {
  GLuint db;
  glGenRenderbuffers(1, &db);
  glBindRenderbuffer(GL_RENDERBUFFER, db);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, db);
  return db;
}

GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height) {
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*) nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texID, 0);
  return texID;
}
