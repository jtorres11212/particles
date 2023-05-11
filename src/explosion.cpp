// Bryn Mawr College,alinen,2020
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
    setWindowSize(1000,1000);
    createConfetti(500);
    renderer.loadShader("billboard-animated",
      "../shaders/billboard-animated.vs",
      "../shaders/billboard-animated.fs");
    renderer.loadTexture("explosion",
      "../textures/brolysprite3.png",0);
    renderer.loadTexture("particle", "../textures/ParticleFlare.png", 0);
    renderer.loadTexture("shot", "../textures/blast.png", 0);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
    lookPos.x += 0.1f;
    lookPos.y-=0.1f;
    eyePos.y-=0.1f;
    shot.pos = vec3(0.0,0.0,0.3);
    shot.vel = vec3(-0.03, 0, 0.0);
    shot.color = vec4(1.0, 1.0, 1.0, 1.0);
    shot.size=0.25f;


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
    for (int i=0; i < size; i++){
      Particle particle;
      particle.color = vec4(0.47f,0.87f,0.47f, 1);
      particle.size = 0.25;
      particle.rot = 1;
      particle.pos = agl::randomUnitSphere();
      updatevel(particle.pos.x, particle.pos.y, particle.pos.z);
      particle.vel = vec3(vx, vy, vz);
      particle.pos = particle.vel* 2.0f;
      particle.vel = vec3(-0.33,-0.13,0) - particle.pos; // 0,0,0
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
       if(particle.pos.x < 0.4 && particle.pos.x > -0.4 && particle.pos.y < 0.4 && particle.pos.y > -0.4){ //&& particle.pos.z < 0.1 && particle.pos.z > -0.1
        printf("disappeared\n");
         mParticles.erase(mParticles.begin() + i);
         i--;
         continue;
      }else{
        particle.pos += particle.vel*dt*vec3(.5,.5,.5);
        mParticles[i]=particle;
      }
      if(mParticles.size() < 50){
      printf("here\n");
      collected = true;
    }
    if(collected){
      shot.pos += shot.vel;
    if(particle.pos.x < 0.1 && particle.pos.z > -0.1 && particle.pos.y < 0.1 && particle.pos.y > -0.1
      && particle.pos.z < 0.1 && particle.pos.z > -0.1){
        //shot.color.a +=0.01;
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
      renderer.push();
      renderer.scale(vec3(0.5, 1, 1));
      renderer.texture("image", "shot");
      renderer.sprite(shot.pos, shot.color, 1.0, 0.0);
      renderer.pop();
    }

    // draw
    renderer.texture("image", "particle");
    for (int i=0; i < mParticles.size(); i++)
    {
      float c = 0.25;
      Particle particle=mParticles[i];
      renderer.sprite(particle.pos*c, particle.color, particle.size, particle.rot); //vec4(0.62f,0.27f,0.69f,1)
      //renderer.sprite(-particle.pos*c,particle.color, .1f, .2f); // 1 1 1 1
      //renderer.sprite((particle.pos-vec3(0.5,0.5,0.5)*c), particle.color, 1, 1); //vec4(0.26f,0.02f,0.43f,1)
      //renderer.sprite((particle.pos-vec3(.25,0.75,.25)*c), particle.color, .2f, 1); //vec4(0.26f,0.0f,0.54f,1)
      //renderer.sprite((particle.pos-vec3(.1,0.6,0.6)*c), particle.color, .2f, 1); //vec4(1,0.89f,0.94f,1)
    }
  }

  void mouseMotion(int x,int y,int dx,int dy) {
  }

  void mouseDown(int button,int mods) {
  }

  void mouseUp(int button,int mods) {
  }

  void scroll(float dx,float dy) {
    eyePos.z += dy;
  }

  void draw() {

    // 30 fps => each frame 1/30 long,e.g. when time=1s,we play frame 30
    if(frame < 64){
      frame=elapsedTime() * fps;
    }else{
      renderer.beginShader("sprite");
      updateConfetti(dt());
      drawConfetti();
      renderer.endShader();
    }
    int _frames=int(frame) % 64;
    _frames = 64 - _frames;
    int _row= _frames / numCols;
    int _col= _frames % numCols;

    renderer.beginShader("billboard-animated");
    renderer.texture("image","explosion");

    

    
    renderer.setUniform("Frame",_frames);
    renderer.setUniform("Rows",numRows);
    renderer.setUniform("Cols",numCols);

    renderer.setUniform("frows",_row);
    renderer.setUniform("fcols",_col);


    float aspect=((float)width()) / height();
    renderer.perspective(glm::radians(60.0f),.5*aspect,0.1f,50.0f);

    renderer.lookAt(eyePos,lookPos,up);
    renderer.scale(vec3(0.5, 1.0, 1.0));
    renderer.sprite(vec3(0.33, 0.13, 0.0)*1.2f,vec4(1.0f),1.0);

    renderer.endShader();
  }

protected:

  vec3 eyePos=vec3(0,0,2);
  vec3 lookPos=vec3(0,0,0);
  vec3 up=vec3(0,1,0);
  float vx,vy,vz;
  bool collected = false;
  Particle shot;
  int frame=0;
  int numRows=4;
  int numCols=4;
  int fps=24;
  std::vector<Particle> mParticles;
};

int main(int argc,char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}