#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Vector2.h"
#include "gl_canvas2d.h"
#include <vector>

struct Particle
{
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    float maxLifetime;
};

class ParticleSystem
{
  private:
    std::vector<Particle> particles;

  public:
    void createExplosion(const Vector2 &center, int count = 20)
    {
        for (int i = 0; i < count; i++) {
            Particle p;
            p.position = center;
            p.velocity = Vector2((rand() % 100 - 50) / 10.0f, (rand() % 100 - 50) / 10.0f);
            p.maxLifetime = 0.5f + (rand() % 100) / 100.0f;
            p.lifetime = p.maxLifetime;
            particles.push_back(p);
        }
    }

    void update(float deltaTime)
    {
        for (auto it = particles.begin(); it != particles.end();) {
            it->position += it->velocity * deltaTime;
            it->lifetime -= deltaTime;

            if (it->lifetime <= 0) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }
    }

    void render()
    {
        for (auto &p : particles) {
            float alpha = p.lifetime / p.maxLifetime;
            CV::color(1.0f, 0.5f, 0.0f, alpha); // Laranja
            CV::circleFill(p.position.x, p.position.y, 3 * alpha, 6);
        }
    }
};
#endif
