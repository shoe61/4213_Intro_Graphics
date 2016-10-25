#include "Cube.h"


Cube::Cube()
{
   // Vertices of a unit cube centered at origin, sides aligned with axes
   // This is modified from the original in that the vertices are specified by
   // assignment statements.

   vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
   vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
   vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
   vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
   vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
   vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
   vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
   vertices[7] = point4(0.5, -0.5, -0.5, 1.0);


   // RGBA colors
   // This is modified from the original in that the vertices are specified by
   // assignment statements. (VS 2013?)

   vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0);  // black
   vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0);  // red
   vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0);  // yellow
   vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0);  // green
   vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0);  // blue
   vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0);  // magenta
   vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0);  // white
   vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0);   // cyan

   //calls private method quad to divide the six faces into 2 triangles each.
   //The outward facing faces, e.g. 1,0,3,2 are those for which the right- 
   //hand rule applies: if the fingers of the right hand are curled in the 
   //direction of the vertex traversal, the thumb points outward.
   quad(1, 0, 3, 2);
   quad(2, 3, 7, 6);
   quad(3, 0, 4, 7);
   quad(6, 5, 1, 2);
   quad(4, 5, 6, 7);
   quad(5, 4, 0, 1);
}


Cube::~Cube()
{
}

void Cube::quad(int a, int b, int c, int d)
{
   colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
   colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
   colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
   colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
   colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
   colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

void Cube::load(GLuint program)
{
   // Create and initialize a buffer object
   GLuint buffer;
   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors),
      NULL, GL_STATIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

   GLuint vPosition = glGetAttribLocation(program, "vPosition");
   glEnableVertexAttribArray(vPosition);
   glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(0));

   GLuint vColor = glGetAttribLocation(program, "vColor");
   glEnableVertexAttribArray(vColor);
   glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(sizeof(points)));
}

//the draw method doesn't have much to do:
void Cube::draw()
{
   glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}


