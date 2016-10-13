//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

//http://people.sc.fsu.edu/%7Ejburkardt/data/obj/obj.html

#include "Angel.h"

#include "Object.h"

#pragma comment(lib, "freeglut")
#pragma comment(lib, "glew32")



// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint  theta;  // The location of the "theta" shader uniform variable

vector<string> models = { "cube.obj", "gourd.obj", "jack.obj", "table.obj" };

int modelChoice;


char* select()
{
   char* drawIt;

   cout << "Please select the model you'd like to see: \n\t0: a cube with 8 vertices drawn in blender";
   cout << "\n\t1: a gourd with 326 vertices downloaded from http://people.sc.fsu.edu/%7Ejburkardt/data/obj/obj.html";
   cout << "\n\t2: a jack with 2312 vertices drawn in blender\n\n";

   cin >> modelChoice;

   switch (modelChoice)
   {
   case 0: drawIt = "cube.obj";
      break;
   case 1: drawIt = "gourd.obj";
      break;
   case 2: drawIt = "jack.obj";
      break;
   default: drawIt = "cube.obj";
   }
   return drawIt;
   
}





//instantiate an object:
Object blok(select());

//----------------------------------------------------------------------------

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
    glUseProgram( program );

    blok.load(program); //program returned by InitShader.cpp

    theta = glGetUniformLocation( program, "theta" );
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------
int frame, fps, time, timebase = 0;
void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glUniform3fv( theta, 1, Theta );

    blok.draw(); // call to draw
    

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

//----------------------------------------------------------------------------

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

//----------------------------------------------------------------------------

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

//----------------------------------------------------------------------------

void
idle( void )
{
    Theta[Axis] += 0.01;

    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Various Objects" );

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
