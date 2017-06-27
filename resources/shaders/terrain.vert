#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vCol;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 color;
out vec2 texCoord;
out vec3 worldNormal;
out vec3 toLightDir;
out vec3 cameraDir;

void main(void) { 
  vec4 worldPosition = modelMatrix * vec4(vPos, 1.0);
  
  texCoord = vTexCoord;
  worldNormal = (modelMatrix * vec4(vNormal, 0.0)).xyz;
  toLightDir = lightPosition - worldPosition.xyz;
  
  // view matrix contains inverse of camera's location
  vec3 cameraPos = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
  cameraDir = cameraPos - worldPosition.xyz;
  
  gl_Position = projectionMatrix * viewMatrix * worldPosition;
}