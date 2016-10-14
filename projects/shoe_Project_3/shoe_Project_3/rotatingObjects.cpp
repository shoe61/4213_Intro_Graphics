// All of the work on this project is my own, except what what was given in
// class. Scott Schumacher:
//
//
//
//
//******************************************************************************
//                Project #3 Rotating Objects
//                Name: Scott Schumacher
//                Intro to Computer Graphics 10/14/2016
//******************************************************************************
// This program is an adaptation of Project 2. Now, instead of explicitly listing
// the vertices of the object to be displayed, the program reads in the object's
// vertex list, normal list, and an index of the vertices used in the object's 
// faces. Also, instead of listing each vertex every time it appears in a face, 
// vertices are indexed and called by their indices. When you run the program, 
// you'll be given the choice of three models- two that I drew in Blender and 
// one downloaded from http://people.sc.fsu.edu/%7Ejburkardt/data/obj/obj.html.
// Colors are assigned according to the coordiantes of the vertex to which 
// they're assigned.
//
//******************************************************************************

#include "Object.h"

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint  theta;  // The location of the "theta" shader uniform variable

//******************************************************************************

// char* select() prompts the user to select an object file to be instantiated.
// A switch sets the user's choice as char* drawIt, which is sent as an argument 
// to the the Object constructor.

char* select()
{
   int modelChoice;  // switch variable
   char* drawIt;     // argument to constructor

   cout << "Please select the model you'd like to see: \n\t0: a cube with 8 vertices drawn in blender";
   cout << "\n\t1: a teapot with 530 vertices downloaded from http://people.sc.fsu.edu/%7Ejburkardt/data/obj/obj.html";
   cout << "\n\t2: a jack with 2312 vertices drawn in blender\n\n";
   cout << "Type 0, 1, or 2 and press ENTER: ";
   cin >> modelChoice;

   switch (modelChoice)
   {
   case 0: drawIt = "cube.obj";
      break;
   case 1: drawIt = "teapot.obj";
      break;
   case 2: drawIt = "jack.obj";
      break;
   default: drawIt = "cube.obj";
   }
   return drawIt;
}

//instantiate an object called Model from a file selected by the user in char* select():
Object Model(select());

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

    Model.load(program); //program returned by InitShader.cpp

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

    Model.draw(); // call to draw
    

    // Timing etc
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    char display_string[100];
    if (time - timebase>1000) {
       fps = frame*1000.0 / (time - timebase);
       sprintf(display_string, "Schumacher's rotating objects : FPS : %d ", fps);
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
    Theta[Axis] += 0.03;

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
