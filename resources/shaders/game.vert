#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vCol;

uniform vec3 lightPosition[4];
uniform vec3 lightColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform float useFakeLighting;

uniform float numTextureRows;
uniform vec2 textureAtlasXYOffset;

out vec2 texCoord;
out vec3 worldNormal;
out vec3 toLightDir[4];
out vec3 cameraDir;
out float visibility;

const float fogDensity = 0.0;
const float fogGradient = 5.0;

uniform vec4 horizontalClipPlane;

void main(void) {
  // if a model is planar (like grass)
  // but needs to be lit appropriately
  vec3 actualNormal = vNormal;
  
  if (useFakeLighting > 0.5) {
    actualNormal = vec3(0.0, 1.0, 0.0);
  }
  
  vec4 worldPosition = modelMatrix * vec4(vPos, 1.0);
  
  gl_ClipDistance[0] = dot(worldPosition, horizontalClipPlane);
  
  texCoord = (vTexCoord / numTextureRows) + textureAtlasXYOffset;
  worldNormal = (modelMatrix * vec4(actualNormal, 0.0)).xyz;
  
  for (int i = 0; i < 4; i++) {
    toLightDir[i] = lightPosition[i] - worldPosition.xyz;
  }
  
  // view matrix contains inverse of camera's location
  vec3 cameraPos = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
  
  // vector from vertex position pointing to camera
  cameraDir = cameraPos - worldPosition.xyz;
  
  vec4 positionRelativeToCamera = viewMatrix * worldPosition;
  
  // exponential fog calculations
  float dis = length(positionRelativeToCamera.xyz);
  visibility = exp(-pow((dis * fogDensity), fogGradient));
  visibility = clamp(visibility, 0.0, 1.0);  
  gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
