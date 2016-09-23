// All of the work on this project is my own. Scott Schumacher:
//
//
//
//
//******************************************************************************
//                Project #2 Color Cube
//                Name: Scott Schumacher
//                Intro to Computer Graphics 9/23/2016
//******************************************************************************
// This program is an adaptation of source code from the Angel; it has been mod-
// ified by encapsulating all the elements necessary to generate a cube in a
// class. Most of the modification involved rearranging existing code and re-
// directing function calls.
//******************************************************************************
//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projection.

#include "Angel.h"

#pragma comment(lib, "freeglut")
#pragma comment(lib, "glew32")

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

//******************************************************************************

class Cube
   {
   
   private:
      int Index = 0;
      static const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
      point4 points[NumVertices];
      color4 colors[NumVertices];
      
      //vertices      
      point4 vertices[8];

      // RGBA colors
      color4 vertex_colors[];
      
      void quad(int, int, int, int);

   public:
      
      Cube();
      
      void load(GLuint);
      
      void draw();

      ~Cube();
   
   };

//******************************************************************************

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

//******************************************************************************

void Cube::quad(int a, int b, int c, int d)
   {
   colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
   colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
   colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
   colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
   colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
   colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
   }

//******************************************************************************

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

//******************************************************************************

//the draw method doesn't have much to do:
void Cube::draw()
{
   glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

//******************************************************************************

//destructor
Cube::~Cube()
{
  
}

//******************************************************************************

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

//Instantiate a cube:
Cube cube;

GLuint  theta;  // The location of the "theta" shader uniform variable

//******************************************************************************

// OpenGL initialization
void
init()
{
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader_a4.glsl", "fshader_a4.glsl" );
    glUseProgram(program);
        
    cube.load(program); //program returned by InitShader.cpp

    theta = glGetUniformLocation( program, "theta" );
    

    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//******************************************************************************

int frame, fps, time, timebase = 0;
void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glUniform3fv( theta, 1, Theta );

    cube.draw(); // call to draw

    // Timing etc
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    char display_string[100];
    if (time - timebase>1000) {
       fps = frame*1000.0 / (time - timebase);
       sprintf(display_string, "Simpson’s rotating cube : FPS : %d ", fps);
       glutSetWindowTitle(display_string);
       timebase = time;
       frame = 0;
    }

    glutSwapBuffers();
}

//******************************************************************************

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
}

//******************************************************************************

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
	switch( button ) {
	    case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
	    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
	    case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
	}
    }
}

//******************************************************************************

void
idle( void )
{
    Theta[Axis] += 0.025;

    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

//******************************************************************************

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );

    glewExperimental = GL_TRUE;
    glewInit();

    init();
 
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    glutMainLoop();
    return 0;
}
