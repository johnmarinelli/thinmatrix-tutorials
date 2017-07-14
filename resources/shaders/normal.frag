#version 330 

const int NUM_LIGHTS = 4;

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 toLightDir[NUM_LIGHTS];
in vec3 toCameraDir;
in float visibility;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform vec3 lightColor[NUM_LIGHTS];
uniform vec3 attenuations[NUM_LIGHTS];

uniform float shineDamper;
uniform float reflectivity;

uniform float ambientFactor;
uniform vec3 skyColor;

void main() {
  vec4 c = texture(textureSampler, texCoord);
  vec3 l = lightColor[0];
  vec3 a = attenuations[0];
  float s = shineDamper * reflectivity * ambientFactor;
  vec3 ss = skyColor;
  
  fragColor = vec4(0.0);
}
