//******************************************************************************
//                Project #0 Two Dimensional Sierpinski Gasket
//                Name: Scott Schumacher
//                Intro to Computer Graphics 9/7/2016
//******************************************************************************
//      Given the vertices of a triangle, this program selects at random an 
//      interior point, selects at random one of the vertices of the triangle,
//      and computes midpoint, which becomes the basis for generating the next
//      point. This process is repeated for the number of points specified by 
//      numPoints. More points (50K vice 5K) yields a higher resolution product.
//******************************************************************************
//
//
//


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

const int NumPoints = 50000;

//----------------------------------------------------------------------------

void
init( void )
{
    vec2 points[NumPoints];

    // Specifiy the vertices for a triangle
    vec2 vertices[3] = {
        vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )
    };

    // Select an arbitrary initial point inside of the triangle
    points[0] = vec2( 0.25, 0.50 );

    // compute and store N-1 new points
    for ( int i = 1; i < NumPoints; ++i ) {
        int j = rand() % 3;   // pick a vertex at random

        // Compute the point halfway between the selected vertex
        //   and the previous point
        points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
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
    glDrawArrays( GL_POINTS, 0, NumPoints );    // draw the points
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

    glutCreateWindow( "Sierpinski Gasket" );
	//the following code was given in class:
	glewExperimental = GL_TRUE;

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
