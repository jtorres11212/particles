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
  vec3 pos;
  vec3 vel;
  vec4 color;
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
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
    shot.pos = vec3(0,0,0);
    shot.vel = vec3(0.0, 0, 0.03);
    shot.color = vec4(0.2, 1.0, 0.3, 0.3);
    shot.size=0.25f;

    renderer.loadTexture("particle", "../textures/ParticleFlare.png", 0);
  }

void updatevel(float px,float py,float pz)
{
    float dx=0-px;
    float dy=0-py;
    float dz=0-pz;
    float distance=std::sqrt(dx*dx+dy*dy+dz*dz);
    float forcemagnitude=2.0f/distance;
    float directionx=dx/distance;
    float directiony=dy/distance;
    float directionz=dz/distance;
    float forcex=forcemagnitude*directionx;
    float forcey=forcemagnitude*directiony;
    float forcez=forcemagnitude*directionz;
    vx+=forcex;
    vy+=forcey;
    vz+=forcez;
}
  void createConfetti(int size){
    renderer.loadTexture("particle", "../textures/ParticleFlare.png", 0);
    for (int i=0; i < size; i++){
      Particle particle;
      particle.color = vec4(0.47f,0.87f,0.47f, 1);
      particle.size = 0.25;
      particle.rot = 1;
      particle.pos = agl::randomUnitSphere();
      updatevel(particle.pos.x, particle.pos.y, particle.pos.z);
      particle.vel = vec3(vx, vy, vz);
      particle.pos = particle.vel* 2.0f;
      particle.vel = vec3(0,0,0) - particle.pos;
      if (particle.pos.x==0 && particle.pos.y==0 && particle.pos.z==0){
        particle.vel = vec3(0.0f, 0.0f, 0.0f);
        particle.color = vec4(0.1f,0.8f,0.9f,1);
      }
      
      vx=0;
      vy=0;
      vz=0;
      mParticles.push_back(particle);
    }
}

  void updateConfetti(float dt)
  {
    for (int i=0; i < mParticles.size(); i++)
    {
      Particle particle=mParticles[i];
      if(particle.pos.x < 0.1 && particle.pos.z > -0.1 && particle.pos.y < 0.1 && particle.pos.y > -0.1
      && particle.pos.z < 0.1 && particle.pos.z > -0.1){
         mParticles.erase(mParticles.begin() + i);
         i--;
         continue;
      }else{
        particle.pos += particle.vel*dt*vec3(.1,.1,.1);
        mParticles[i]=particle;
      }
      if(mParticles.size() < 20){
      printf("here\n");
      collected = true;
    }
    if(collected){
      shot.pos += shot.vel*.8f;
    if(particle.pos.x < 0.1 && particle.pos.z > -0.1 && particle.pos.y < 0.1 && particle.pos.y > -0.1
      && particle.pos.z < 0.1 && particle.pos.z > -0.1){
        shot.size +=0.1;
    }
  }
  }
    

    
    
  }

  void drawConfetti()
  {
    vec3 cameraPos=renderer.cameraPosition();

    // sort
    for (int i=1; i < mParticles.size(); i++)
    {
      Particle particle1=mParticles[i];
      Particle particle2=mParticles[i - 1];
      float dSqr1=length2(particle1.pos - cameraPos);
      float dSqr2=length2(particle2.pos - cameraPos);
      if (dSqr2 < dSqr1)
      {
        mParticles[i]=particle2;
        mParticles[i - 1]=particle1;
      }
    }
    if(collected){
      renderer.texture("image", "particle");
      renderer.sprite(shot.pos, shot.color, 10.0, 0.0);
    }

    // draw
    renderer.texture("image", "particle");
    for (int i=0; i < mParticles.size(); i++)
    {
      Particle particle=mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot); //vec4(0.62f,0.27f,0.69f,1)
      renderer.sprite(-particle.pos,particle.color, .1f, .2f); // 1 1 1 1
      renderer.sprite((particle.pos-vec3(1,1,1)), particle.color, 1, 1); //vec4(0.26f,0.02f,0.43f,1)
      renderer.sprite((particle.pos-vec3(.5,1.5,.5)), particle.color, .2f, 1); //vec4(0.26f,0.0f,0.54f,1)
      renderer.sprite((particle.pos-vec3(.2,1.2,1.2)), particle.color, .2f, 1); //vec4(1,0.89f,0.94f,1)
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
    float aspect=((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);
    updateConfetti(dt());
    drawConfetti();
    renderer.endShader();
  }

protected:

  vec3 eyePos=vec3(10, 0, 0);
  vec3 lookPos=vec3(0, 0, 0);
  vec3 up=vec3(0, 1, 0);
  float vx,vy,vz;
  bool collected = false;
  Particle shot;

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}