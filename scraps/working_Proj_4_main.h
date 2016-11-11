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
// Colors are assigned according to the coordinates of the vertex to which 
// they're assigned.
//
//******************************************************************************

#include "Object.h"

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat angularSpeed = 5.0 * DegreesToRadians; // amount by which theta is incremented per click

GLuint modelViewLoc; //   SS Model-view matrix uniform shader variable location

vec4 eye, at, up, ctr_box;  // SS
GLfloat BBoxMax, z_eye = 4.0;

GLuint ProjectionLoc; //  SS projection matrix uniform shader variable location

mat4 projection; // projection matrix and assoc variables

GLfloat fovy = 30.0; // field of view, degrees, y- axis: start at 45
GLfloat zFar, zNear, aspectRatio;  

//******************************************************************************

// char* select() prompts the user to select an object file to be instantiated.
// A switch sets the user's choice as char* drawIt, which is sent as an argument 
// to the the Object constructor.

char* select()
{
   int modelChoice;  // switch variable
   char* drawIt;     // argument to constructor

   cout << "Please select the model you'd like to see: \n\n\t0: a cube with 8 vertices";
   cout << " drawn in blender";
   cout << "\n\t1: a teapot with 530 vertices downloaded from";
   cout << " http://people.sc.fsu.edu/%7Ejburkardt/data/obj/obj.html";
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
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   // Load shaders and use the resulting shader program
   GLuint program = InitShader("vshader42.glsl", "fshader42.glsl");
   glUseProgram(program);

   Model.load(program); //program returned by InitShader.cpp

   // this is testing the bounding_box method:
   cout << "bounding box x, including 20 percent: " << (Model.bounding_box()).x << endl;
   cout << "bounding box y, including 20 percent: " << (Model.bounding_box()).y << endl;
   cout << "bounding box z, including 20 percent: " << (Model.bounding_box()).z << endl;

   // this is to check the bounding_box_max method:
   // it's padded!
   cout << "BBox max = " << (Model.bounding_box_max()) << endl;
   

   // this is to check the ctr_box method:
   cout << "bounding box center x: " << (Model.ctr_box()).x << endl;
   cout << "bounding box center y: " << (Model.ctr_box()).y << endl;
   cout << "bounding box center z: " << (Model.ctr_box()).z << endl;

   // SSX theta = glGetUniformLocation(program, "theta");
   
   ProjectionLoc = glGetUniformLocation(program, "projection");  // SS
   modelViewLoc = glGetUniformLocation(program, "modelViewLoc");  // SS


 
      
   
   

   glEnable(GL_DEPTH_TEST);
   glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

// SSX rotation matrix for x axis
mat4 RotX(GLfloat theta)
{
   return mat4(1.0, 0.0, 0.0, 0.0,
      0.0, cos(theta), sin(theta), 0.0,
      0.0, -sin(theta), cos(theta), 0.0,
      0.0, 0.0, 0.0, 1.0);
}

// SSX rotation matrix for y axis
mat4 RotY(GLfloat theta)
{
   return mat4(cos(theta), 0.0, sin(theta), 0.0,
      0.0, 1.0, 0.0, 0.0,
      -sin(theta), 0.0, cos(theta), 0.0,
      0.0, 0.0, 0.0, 1.0);
}

// SSX rotation matrix for z axis
mat4 RotZ(GLfloat theta)
{
   return mat4(cos(theta), -sin(theta), 0.0, 0.0,
      sin(theta), cos(theta), 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------
int frame, fps, time, timebase = 0;

void
display(void)
{
   mat4 model_view; //SS model view is changeable so is declared right before draw
   mat4 Rotato = RotX(Theta[Xaxis]) * RotY(Theta[Yaxis]) * RotZ(Theta[Zaxis]); //SSX

   eye = vec4(0.0, 0.0, z_eye, 1.0); // SS
   at = Model.ctr_box(); // SS
   up = vec4(0.0, 3.0, 0.0, 0.0);  // SS

   model_view = LookAt(eye, at, up);  // SS
   model_view *= Rotato; // SSX

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // SSX glUniform3fv(theta, 1, Theta);

   // SS
   glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view);
   

   Model.draw(); // call to draw


   // Timing etc
   frame++;
   time = glutGet(GLUT_ELAPSED_TIME);
   char display_string[100];
   if (time - timebase > 1000) {
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
keyboard(unsigned char key, int x, int y)
{
   switch (key) 
   {
   case 'z': z_eye *= 1.1;  break; // SSZ
   case 'Z': z_eye *= 0.9; break; // SSZ

   case 033: // Escape Key
   case 'q': case 'Q':
      exit(EXIT_SUCCESS);
      break;

   
   

 
   }
}

//----------------------------------------------------------------------------

void  // SSX
mouse(int button, int state, int x, int y)
{
   if (state == GLUT_DOWN) 
   {
      switch (button)
      {
      case GLUT_LEFT_BUTTON:    Axis = Xaxis; Theta[Axis] += angularSpeed; 
         if(Theta[Axis] > 360.0){Theta[Axis] -= 360.0;} break;
      case GLUT_MIDDLE_BUTTON:  Axis = Yaxis; Theta[Axis] += angularSpeed;
         if(Theta[Axis] > 360.0){ Theta[Axis] -= 360.0;} break;
      case GLUT_RIGHT_BUTTON:   Axis = Zaxis; Theta[Axis] += angularSpeed;
         if(Theta[Axis] > 360.0){Theta[Axis] -= 360.0;} break;
      }
   }
}

//----------------------------------------------------------------------------

void
idle(void)
{
   //Theta[Axis] += 0.03;

   

   glutPostRedisplay();
}

//----------------------------------------------------------------------------


void // SS
reshape(int width, int height)
{
   glViewport(0, 0, width, height);
   aspectRatio = GLfloat (width) / height;

   BBoxMax = (Model.bounding_box_max());
   // SSZ zFar = z_eye + BBoxMax / 2;
   // SSZ zNear = zFar - BBoxMax;
   zFar = 22;
   zNear = 0.5;

   projection = Perspective(fovy, aspectRatio, zNear, zFar); // SSZ
   glUniformMatrix4fv(ProjectionLoc, 1, GL_TRUE, projection); // SSZ
}


//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(512, 512);
   glutInitContextVersion(3, 2);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutCreateWindow("Various Objects");

   glewExperimental = GL_TRUE;

   glewInit();
   init();

   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   // SSX glutIdleFunc(idle);
   glutReshapeFunc(reshape); //SS

   glutMainLoop();

   return 0;
}
