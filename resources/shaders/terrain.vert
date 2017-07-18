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
uniform vec4 horizontalClipPlane;

out vec3 color;
out vec2 texCoord;
out vec3 worldNormal;
out vec3 toLightDir[4];
out vec3 cameraDir;
out float visibility;

const float fogDensity = 0.007;
const float fogGradient = 5.0;

void main(void) { 
  vec4 worldPosition = modelMatrix * vec4(vPos, 1.0);
  
  texCoord = vTexCoord;
  worldNormal = (modelMatrix * vec4(vNormal, 0.0)).xyz;

  gl_ClipDistance[0] = dot(worldPosition, horizontalClipPlane);

  for (int i = 0; i < 4; ++i) {
    toLightDir[i] = lightPosition[i] - worldPosition.xyz;
  }
  
  // view matrix contains inverse of camera's location
  vec3 cameraPos = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
  cameraDir = cameraPos - worldPosition.xyz;
  
  vec4 positionRelativeToCamera = viewMatrix * worldPosition;
  float dis = length(positionRelativeToCamera.xyz);
  visibility = exp(-pow((dis * fogDensity), fogGradient));
  visibility = clamp(visibility, 0.0, 1.0);
  
  // TODO: temporary fix.  fix visibility after everything is done
  visibility = 1.0;
  
  gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
