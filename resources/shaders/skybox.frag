#version 330

in vec3 textureCoords;
out vec4 outColor;

uniform samplerCube dayCubeMap;
uniform samplerCube nightCubeMap;

uniform vec3 fogColor;
uniform float blendFactor;

const float lowerLimit = 0.0;
const float upperLimit = 0.01;

void main() {
  vec4 dayTexture = texture(dayCubeMap, textureCoords);
  vec4 nightTexture = texture(nightCubeMap, textureCoords);
  
  vec4 finalColor = mix(dayTexture, nightTexture, blendFactor);
  
  float factor = (textureCoords.y - lowerLimit) / (upperLimit - lowerLimit);
  factor = clamp(factor, 0.0, 1.0);
  outColor = mix(vec4(fogColor, 1.0), finalColor, factor);
  outColor = finalColor;
}
