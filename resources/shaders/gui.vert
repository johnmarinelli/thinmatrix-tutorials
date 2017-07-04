#version 330

layout (location = 0) in vec2 position;
uniform mat4 modelMatrix;
out vec2 textureCoords;

void main(void){
  gl_Position = modelMatrix * vec4(position, 0.0, 1.0);
  textureCoords = vec2((position.x+1.0)/2.0, (position.y+1.0)/2.0);
}
