#version 330

in vec2 fTexCoord;
out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 col;

void main() {
  vec3 c = col;
  vec4 t = texture(tex, fTexCoord);
  fragColor = vec4(1.0, 1.0, 1.0, t * vec4(c, 1.0));
}
