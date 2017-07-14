#version 330

layout (location = 0) in vec3 position;
out vec3 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
  vec4 pos = projectionMatrix * viewMatrix * vec4(position, 1.0);
  textureCoords = position;
  
  // divide by w component to optimize depth buffer processing
  // this way, skybox will have a max z value of 1.0
  // which will fail the depth test whenever there's
  // another object in front
  gl_Position = pos.xyww;
}
