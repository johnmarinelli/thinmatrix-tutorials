#version 330

in vec4 clipSpaceCoords;
in vec2 fTexCoords;
in vec3 fToCameraVector;
in vec3 fFromLightVector;

out vec4 fragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D refractionDepthTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform float moveFactor;
uniform vec3 lightColor;

const float distortionStrength = 0.02;
const float shineDamper = 20.0;
const float reflectivity = 0.6;

void main(void) {
  // clip space -> ndc
  vec2 ndc = (clipSpaceCoords.xy / clipSpaceCoords.w);
  ndc /= 2;
  ndc += 0.5;
  
  vec2 refractTexCoords = ndc;
  vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
  
  // depth will be from 0 - 1 on a logarithmic scale
  // so we need to remap it to a linear scale
  float depth = texture(refractionDepthTexture, refractTexCoords).r;
  float near = 0.1;
  float far = 1000.0;
  
  // distance from camera to bottom of water
  float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
  
  depth = gl_FragCoord.z;
  // distance from camera to top of water
  float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
  
  float waterDepth = floorDistance - waterDistance;
  
  // distortion values are stored only in rg
  vec2 distortedTexCoords = texture(dudvMap, vec2(fTexCoords.x + moveFactor, fTexCoords.y)).rg*0.1;
  distortedTexCoords = fTexCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+moveFactor);
  // clamp() code dampens distortion a bit
  vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * distortionStrength * clamp(waterDepth / 20.0, 0.0, 1.0);
  
  // clamp coordinates to avoid texture wrapping glitch
  refractTexCoords += clamp(totalDistortion, 0.001, 0.999);
  reflectTexCoords += totalDistortion;
  reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
  reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
  
  vec4 reflectionColor = texture(reflectionTexture, reflectTexCoords);
  vec4 refractionColor = texture(refractionTexture,  refractTexCoords);
  
  // normal map
  // sample at the *distorted* texture coordinates
  vec4 normalMapValue = texture(normalMap, distortedTexCoords);
  vec3 normal = vec3(normalMapValue.r * 2.0 - 1.0, normalMapValue.b * 3.0 /* make normal point a little more upwards */, normalMapValue.g * 2.0 - 1.0);
  normal = normalize(normal);
  
  // fresnel effect
  vec3 normToCameraVector = normalize(fToCameraVector);
  vec3 waterSurfaceNormal = normal;
  float refractiveFactor = dot(normToCameraVector, waterSurfaceNormal);
  
  float reflectiveCoefficient = 1.0;
  refractiveFactor = pow(refractiveFactor, reflectiveCoefficient);
  
  // specular lighting
  vec3 reflectedLightDir = reflect(normalize(fFromLightVector), normal);
  float specularFactor = dot(reflectedLightDir, normToCameraVector);
  specularFactor = max(specularFactor, 0.0);
  specularFactor = pow(specularFactor, shineDamper);
  // clamp() code dampens specularity a bit
  vec3 specular = reflectivity * lightColor * specularFactor * clamp(waterDepth / 5.0, 0.0, 1.0);
  
  fragColor = mix(reflectionColor, refractionColor, refractiveFactor);
  fragColor = mix(fragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specular, 0.0);
  fragColor.a = clamp(waterDepth / 5.0, 0.0, 1.0);
}
