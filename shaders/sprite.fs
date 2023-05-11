#version 400

in vec2 uv;
in vec4 col;

uniform sampler2D image;
out vec4 FragColor;

void main()
{
  vec4 c=col*texture(image,uv);
  FragColor=c;
}