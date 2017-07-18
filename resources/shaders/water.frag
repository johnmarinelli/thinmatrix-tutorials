#version 330

in vec4 clipSpaceCoords;
in vec2 fTexCoords;
in vec3 fToCameraVector;

out vec4 fragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform float moveFactor;
const float distortionStrength = 0.02;

void main(void) {
  // clip space -> ndc
  vec2 ndc = (clipSpaceCoords.xy / clipSpaceCoords.w);
  ndc /= 2;
  ndc += 0.5;
  
  vec2 refractTexCoords = ndc;
  vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
  
  // distortion is stored only in rg
  vec2 distortion1 = (texture(dudvMap, vec2(fTexCoords.x + (moveFactor), fTexCoords.y)).rg * 2.0 - 1.0) * distortionStrength;
  vec2 distortion2 = (texture(dudvMap, vec2(-fTexCoords.x + (moveFactor), fTexCoords.y + moveFactor)).rg * 2.0 - 1.0) * distortionStrength;
  vec2 totalDistortion = distortion1 + distortion2;
  
  // clamp coordinates to avoid texture wrapping glitch
  refractTexCoords += clamp(totalDistortion, 0.001, 0.999);
  reflectTexCoords += totalDistortion;
  reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
  reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
  
  vec4 reflectionColor = texture(reflectionTexture, reflectTexCoords);
  vec4 refractionColor = texture(refractionTexture,  refractTexCoords);
  
  // fresnel effect
  vec3 viewVector = normalize(fToCameraVector);
  vec3 waterSurfaceNormal = vec3(0.0, 1.0, 0.0);
  float refractiveFactor = dot(viewVector, waterSurfaceNormal);
  
  float reflectiveCoefficient = 10.0;
  refractiveFactor = pow(refractiveFactor, reflectiveCoefficient);
  
  fragColor = mix(reflectionColor, refractionColor, refractiveFactor);
  fragColor = mix(fragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
  
}
