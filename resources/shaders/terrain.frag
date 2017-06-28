#version 330

in vec3 color;
in vec2 texCoord;
in vec3 worldNormal;
in vec3 toLightDir;
in vec3 cameraDir;
in float visibility;
out vec4 fragColor;

uniform sampler2D textureSampler;
uniform vec3 lightColor;

uniform float shineDamper;
uniform float reflectivity;

uniform float ambientFactor;
uniform vec3 skyColor;

void main(void) { 
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
  
  vec3 light = diffuse + specular;
  
  fragColor = vec4(light, 1.0) * texture(textureSampler, texCoord);
  fragColor = mix(vec4(skyColor, 1.0), fragColor, visibility);
}
