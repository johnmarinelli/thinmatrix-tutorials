#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

const int NUM_LIGHTS = 4;

out vec2 texCoord;
out vec3 surfaceNormal;
out vec3 toLightDir[NUM_LIGHTS];
out vec3 toCameraDir;
out float visibility;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPositionEyeSpace[4];

uniform float numTextureRows;
uniform vec2 textureAtlasXYOffset;

const float fogDensity = 0.0;
const float fogGradient = 5.0;

uniform vec4 clipPlane;

void main(void) {
  vec4 worldPosition = modelMatrix * vec4(vPos, 1.0);
  gl_ClipDistance[0] = dot(worldPosition, clipPlane);
  
  mat4 modelViewMatrix = viewMatrix * modelMatrix;
  vec4 positionRelativeToCamera = modelViewMatrix * vec4(vPos * 1.0);
  gl_Position = projectionMatrix * positionRelativeToCamera;
  
  texCoord = (vTexCoord / numTextureRows) + textureAtlasXYOffset;
  surfaceNormal = (modelVieMatrix * vec4(vNormal, 1.0)).xyz;
  
  for (int i = 0; i < NUM_LIGHTS; ++i) {
    toLightDir[i] = lightPositionEyeSpace[i] - positionRelativeToCamera.xyz;
  }
  
  toCameraDir = -positionRelativeToCamera.xyz;
  
  float dis = length(positionRelativeToCamera.xyz);
  visibility = exp(-pow((dis * fogDensity), fogGradient));
  visibility = clamp(visibility, 0.0, 1.0);
}
