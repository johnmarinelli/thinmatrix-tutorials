#version 330

layout (location = 0) in vec2 vPos;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main() {
  gl_Position = projectionMatrix * modelViewMatrix * vec4(vPos, 0.0, 1.0);
}
