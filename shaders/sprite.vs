#version 400

layout (location=0) in vec3 vPosition;

uniform int fR;
uniform int fC;
uniform vec3 cameraPos;
uniform float size;
uniform float R;
uniform vec3 diff;
uniform vec4 rgb;
uniform mat4 MVP;
uniform int f;
uniform int w;
uniform int h;

out vec4 col;
out vec2 uv;
void main()
{
  col=rgb;
  uv=vPosition.xy;
  vec3 z=normalize(cameraPos-diff);
  vec3 x=normalize(cross(vec3(0,1,0),z));
  vec3 y=normalize(cross(z,x));
  mat3 R=mat3(x,y,z);
  x=vec3(cos(fR),-sin(fR),0);
  y=vec3(sin(R),cos(R),0);
  z=vec3(0,0,1);
  mat3 M=mat3(x,y,z);
  uv.x=(vPosition.x+fC)/h;
  uv.y=-(vPosition.y+(fR))/w;

  vec3 eyePos=M*R*size*(vPosition-vec3(0.5,0.5,0.0))+diff;
  gl_Position=MVP*vec4(eyePos,1.0); 
}