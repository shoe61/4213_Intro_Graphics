#include "Angel.h"

#pragma once

class Cube
{
   typedef Angel::vec4  color4;
   typedef Angel::vec4  point4;

private:
   int Index = 0;
   static const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
   point4 points[NumVertices];
   color4 colors[NumVertices];

   // Vertices of a unit cube centered at origin, sides aligned with axes
   point4 vertices[8];

   // RGBA colors
   color4 vertex_colors[8];

   void quad(int, int, int, int);


public:
   Cube();
   ~Cube();
   void load(GLuint);
   void draw();
};

