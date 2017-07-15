#version 330

in vec3 color;
in vec2 texCoord;

// light and camera directions are in tangent space
// because the normalMapValue will be in tangent space
in vec3 toLightDir[4];
in vec3 toCameraDir;
in float visibility;

out vec4 fragColor;

uniform sampler2D modelTexture;
uniform sampler2D normalMap;
uniform vec3 lightColor[4];
uniform vec3 attenuations[4];

uniform float shineDamper;
uniform float reflectivity;

uniform float ambientFactor;

uniform vec3 skyColor;

void main(void) {
  vec4 texColor = texture(modelTexture, texCoord);
  vec4 normalMapValue = 2.0 * texture(normalMap, texCoord) - 1.0;
  
  // fragment is transparent
  if (texColor.a < 0.5) {
    discard;
  }
  
  vec3 totalDiffuse = vec3(0.0);
  vec3 totalSpecular = vec3(0.0);
  vec3 unitCameraDir = normalize(toCameraDir);
  
  for (int i = 0; i < 4; ++i) {
    vec3 unitNormal = normalize(normalMapValue.xyz);
    vec3 unitToLightDir = normalize(toLightDir[i]);
    
    float distanceToLight = length(toLightDir[i]);
    // attenuation[0] + (attenuation[1] * distance) + (at
    float attenuationFactor = attenuations[i].x + (attenuations[i].y * distanceToLight) + (attenuations[i].z * pow(distanceToLight, 2.0));
    vec3 lightDir = -unitToLightDir;
    
    vec3 reflectedLightDir = reflect(lightDir, unitNormal);
    float specularFactor = dot(reflectedLightDir, unitCameraDir);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, shineDamper);
    vec3 specular = dampedFactor * reflectivity * lightColor[i];
    
    float ndot = dot(unitNormal, unitToLightDir);
    float brightness = max(ndot, 0.0);
    vec3 diffuse = brightness * lightColor[i];
    diffuse /= attenuationFactor;
    specular /= attenuationFactor;
    
    totalDiffuse += diffuse;
    totalSpecular += specular;
  }
  
  totalDiffuse = max(totalDiffuse, ambientFactor);
  
  fragColor = vec4(totalDiffuse, 1.0) * texColor + vec4(totalSpecular, 1.0);
  fragColor = mix(vec4(skyColor, 1.0), fragColor, visibility);
   
}
