#version 330

layout (location = 0) in vec4 vCoords; // vec2 pos, vec2 uv

out vec2 fTexCoord;

uniform mat4 projectionMatrix;

void main() {
  gl_Position = projectionMatrix * vec4(vCoords.xy, 0.0, 1.0);
  fTexCoord = vCoords.zw;
}
