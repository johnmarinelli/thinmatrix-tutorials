#version 330

in vec3 color;
in vec2 texCoord;
in vec3 worldNormal;
in vec3 toLightDir[4];
in vec3 cameraDir;
in float visibility;
out vec4 fragColor;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform vec3 lightColor[4];
uniform vec3 attenuations[4];

uniform float shineDamper;
uniform float reflectivity;

uniform float ambientFactor;
uniform vec3 skyColor;

void main(void) {
  float tileFactor = 40.0;
  
  /* 
   * Multitexturing
   */
  vec4 blendMapColor = texture(blendMap, texCoord);
  
  float backgroundTextureAmount = 1.0 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
  vec2 tiledCoords = texCoord * tileFactor;
  vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backgroundTextureAmount;
  vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
  vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
  vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;
  vec4 texColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
  
  vec3 totalDiffuse = vec3(0.0);
  vec3 totalSpecular = vec3(0.0);
  
  /*
   * Lighting
   */
  for (int i = 0; i < 4; ++i) {
    vec3 unitNormal = normalize(worldNormal);
    vec3 unitToLightDir = normalize(toLightDir[i]);
    
    float distanceToLight = length(toLightDir[i]);
    float attenuationFactor = attenuations[i].x + (attenuations[i].y * distanceToLight) + (attenuations[i].z * pow(distanceToLight, 2.0));
    
    // Specular lighting
    vec3 unitCameraDir = normalize(cameraDir);
    vec3 lightDir = -unitToLightDir;
    vec3 reflectedLightDir = reflect(lightDir, unitNormal);
    float specularFactor = dot(reflectedLightDir, unitCameraDir);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, shineDamper);
    vec3 specular = dampedFactor * reflectivity * lightColor[i];
    
    // Diffuse lighting
    float ndot = dot(unitNormal, unitToLightDir);
    float brightness = max(ndot, ambientFactor);
    vec3 diffuse = brightness * lightColor[i];
    
    diffuse /= attenuationFactor;
    specular /= attenuationFactor;
    
    totalDiffuse += diffuse;
    totalSpecular += specular;
  }
  
  totalDiffuse = max(totalDiffuse, ambientFactor);
  
  /*
   * Lighting
   
  vec3 unitNormal = normalize(worldNormal);
  vec3 unitToLightDir = normalize(toLightDir);
  
  // Specular lighting
  vec3 unitCameraDir = normalize(cameraDir);
  vec3 lightDir = -unitToLightDir;
  vec3 reflectedLightDir = reflect(lightDir, unitNormal);
  float specularFactor = dot(reflectedLightDir, unitCameraDir);
  specularFactor = max(specularFactor, 0.0);
  float dampedFactor = pow(specularFactor, shineDamper);
  vec3 specular = dampedFactor * reflectivity * lightColor;
  
  // Diffuse lighting
  float ndot = dot(unitNormal, unitToLightDir);
  float brightness = max(ndot, ambientFactor);
  vec3 diffuse = brightness * lightColor;
   */
  
  fragColor = vec4(totalDiffuse, 1.0) * texColor + vec4(totalSpecular, 1.0);
  fragColor = mix(vec4(skyColor, 1.0), fragColor, visibility);
}
