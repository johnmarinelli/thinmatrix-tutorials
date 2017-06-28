#version 330

in vec3 color;
in vec2 texCoord;
in vec3 worldNormal;
in vec3 toLightDir;
in vec3 cameraDir;
out vec4 fragColor;
in float visibility;

uniform sampler2D textureSampler;
uniform vec3 lightColor;

uniform float shineDamper;
uniform float reflectivity;

uniform float ambientFactor;

uniform vec3 skyColor;

void main(void) {
  vec4 texColor = texture(textureSampler, texCoord);
  
  if (texColor.a < 0.5) {
    discard;
  }
  
  vec3 unitNormal = normalize(worldNormal);
  vec3 unitToLightDir = normalize(toLightDir);
  
  vec3 unitCameraDir = normalize(cameraDir);
  vec3 lightDir = -unitToLightDir;
  vec3 reflectedLightDir = reflect(lightDir, unitNormal);
  float specularFactor = dot(reflectedLightDir, unitCameraDir);
  specularFactor = max(specularFactor, 0.0);
  float dampedFactor = pow(specularFactor, shineDamper);
  vec3 specular = dampedFactor * reflectivity * lightColor;
  
  float ndot = dot(unitNormal, unitToLightDir);
  float brightness = max(ndot, ambientFactor);
  vec3 diffuse = brightness * lightColor;

  fragColor = vec4(diffuse, 1.0) * texColor + vec4(specular, 1.0);
  fragColor = mix(vec4(skyColor, 1.0), fragColor, visibility);
}
