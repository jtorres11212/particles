// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/brolysprite.png", true);
    renderer.loadTexture("tree", img, 0);
    int h = img.height();
    int w = img.width();
    hw=(float)h/(float)w;
    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }


  void mouseMotion(int x, int y, int dx, int dy) {
    if(clk){
          vp=vp-(dy*0.05);
          az=az-(dx*0.05);
          if (vp>6.28){
            vp=0;
          }
          else if(vp<0){
            vp=6.28;
          }
          if (az<-3.14){
            az=.14;
          }
          else if(az>3.14){
            az=-3.14;
          }
          float x=rad*sin(az)*cos(vp);
          float y=rad*sin(vp);
          float z=rad*cos(az)*cos(vp);
          eyePos=vec3(x,y,z);
        }
    }

  void mouseDown(int button, int mods) {
    clk=true;
  }

  void mouseUp(int button, int mods) {
    clk=false;
  }

  void scroll(float dx, float dy) {
    rad=rad+dy;
    float x=rad*sin(az)*cos(vp);
    float y=rad*sin(vp);
    float z=rad*cos(az)*cos(vp);
    eyePos=vec3(x,y,z);
  }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);

    /*/ draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();*/

    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();
    view=normalize(eyePos-lookPos);
    renderer.rotate(atan2(view.x,view.z),vec3(0.0,1.0,0.0));
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();

    renderer.endShader();
  }

protected:
  float hw;
  bool clk=false;
  float rad=5;
  float vp=0;
  float az=0;
  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 view;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
