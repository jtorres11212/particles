// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float size;
  float rot;
  float startTime;
  float ttl;//time to live
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(1000);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }


  void createConfetti(int size)
  {
    renderer.loadTexture("particle", "../textures/particle.png", 0);
    for (int i = 0; i < size; i++)
    {
      Particle particle;
      particle.color = vec4(agl::randomUnitCube(), 1);
      particle.size = 0.25;
      particle.rot = 0.0;
      particle.pos = agl::randomUnitCube();
      particle.vel = agl::randomUnitCube();
      mParticles.push_back(particle);
    }
  }

  void updateConfetti(float dt)
  {
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      particle.pos += particle.vel * dt;
      if (particle.pos.x < -1 || particle.pos.x > 1) particle.vel.x = -particle.vel.x;
      if (particle.pos.y < -1 || particle.pos.y > 1) particle.vel.y = -particle.vel.y;
      if (particle.pos.z < -1 || particle.pos.z > 1) particle.vel.z = -particle.vel.z;
      mParticles[i] = particle;
    }
  }

  void drawConfetti()
  {
    vec3 cameraPos = renderer.cameraPosition();

    // sort
    for (int i = 1; i < mParticles.size(); i++)
    {
      Particle particle1 = mParticles[i];
      Particle particle2 = mParticles[i - 1];
      float dSqr1 = length2(particle1.pos - cameraPos);
      float dSqr2 = length2(particle2.pos - cameraPos);
      if (dSqr2 < dSqr1)
      {
        mParticles[i] = particle2;
        mParticles[i - 1] = particle1;
      }
    }

    // draw
    renderer.texture("image", "particle");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.x += dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    updateConfetti(dt());
    drawConfetti();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(10, 0, 0);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
