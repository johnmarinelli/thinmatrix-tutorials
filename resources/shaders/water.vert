#version 330

layout (location = 0) in vec2 vPos;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPosition;

out vec4 clipSpaceCoords;
out vec2 fTexCoords;
out vec3 fToCameraVector;

const float tiling = 6.0;

void main(void) {
  vec4 worldPosition = modelMatrix * vec4(vPos.x, 0.0, vPos.y, 1.0);
  clipSpaceCoords = projectionMatrix * viewMatrix * worldPosition;
  fTexCoords = vec2(vPos.x / 2.0 + 0.5, vPos.y / 2.0 + 0.5) * tiling;

  fToCameraVector = cameraPosition - worldPosition.xyz;
  gl_Position = clipSpaceCoords;
}
