//******************************************************************************
//                Project #1 Squared Spiral Object
//                Name: Scott Schumacher
//                Intro to Computer Graphics 9/12/2016
//******************************************************************************
// With origin at center of viewing area, draw a counterclockwise squared spiral
// figure to mimic the example in the assignment. 
//******************************************************************************


// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"

//The following lines were given in class. According to 
//https://msdn.microsoft.com/en-us/library/7f0aews7(v=vs.120).aspx, each line
//"Places a library-search record in the object file. This comment type must be 
//accompanied by a commentstring parameter containing the name (and possibly 
//the path) of the library that you want the linker to search."
#pragma comment(lib, "freeglut")
#pragma comment(lib, "glew32")

const int NumPoints = 38;

//----------------------------------------------------------------------------

void
init( void )
{
    vec2 points[NumPoints];

	double x = 0.0;
	double y = 0.0;

   //specify origin at center
	points[1] = vec2( x, y);

    // compute and store Numpoints-1 new points
	double l = 0.1;   //the length of each leg of the spiral
	int k = 1;        //x and y are alternately "incremented" and "decremented"
                     //to accomplish this, int k alternates +1, -1
	int v = 1;        //corner counter
	 
    for ( int i = 1; i < 19; i++) 
	{
		v++;                          //set subscript to next corner in points
		x += k*l;                     //x value augmented by l; y is unchanged
		points[v] = vec2(x, y);       //define new point
		v++;                          //set subscript to next corner in points
		y -= k*l;                     //y is de-augmented; x is unchanged
		points[v] = vec2(x, y);       //define new point
		
		l += 0.1;                     //l is augmented for next two legs
		k *= -1;                      //-k means x will decrease and y increase.
                                    //This changes after every two legs.
    }

	

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader_a2.glsl", "fshader_a2.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
	glDrawArrays(GL_LINE_STRIP, 0, NumPoints);
    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Scott Schumacher Project 1" );
	//the following code was given in class:
	glewExperimental = GL_TRUE;

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
