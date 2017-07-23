#ifndef ParticleMaster_hpp
#define ParticleMaster_hpp

#include <vector>
#include "Loader.hpp"
#include "Particle.hpp"
#include "ParticleRenderer.hpp"

class ParticleMaster {
public:
  std::vector<Particle> mParticles;
  ParticleRenderer mParticleRenderer;
  
  ParticleMaster();
  
  void init(Loader& loader, const glm::mat4& projectionMatrix, Camera& camera);
  
  void update(double dt);
  
  void render();
  
  void cleanUp();
  
  void addParticle(const Particle& particle);
};

#endif
