#include "ParticleMaster.hpp"

ParticleMaster::ParticleMaster() {
}

void ParticleMaster::init(Loader &loader, const glm::mat4 &projectionMatrix, Camera& camera) {
  mParticleRenderer.init(loader, projectionMatrix, camera);
}

// TODO: make this more logical
// (don't remove() in an update loop)
void ParticleMaster::update(double dt) {
  mParticles.erase(std::remove_if(mParticles.begin(), mParticles.end(), [dt] (Particle& p) {
    bool alive = p.update(dt);
    return !alive;
  }), mParticles.end());
}

void ParticleMaster::render() {
  mParticleRenderer.render(mParticles);
}

void ParticleMaster::cleanUp() {
  mParticleRenderer.cleanUp();
}

void ParticleMaster::addParticle(const Particle& particle) {
  mParticles.push_back(particle);
}
