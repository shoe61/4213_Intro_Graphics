//Scott Schumacher
//Project 4: vshader.glsl

#version 420 

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform mat4 projection;

uniform mat4 modelViewLoc;

void main() 
{
    gl_Position = projection * modelViewLoc * vPosition; //SS
    color = vColor;
} 
