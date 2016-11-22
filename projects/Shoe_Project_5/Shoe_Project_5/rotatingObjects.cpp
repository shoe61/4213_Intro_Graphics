// All of the work on this project is my own, except what what was given in
// class, and the wireframe toggle, which I got from Mason Ellis and Christian
// Norfleet. Scott Schumacher:
//
//
//
//
//******************************************************************************
//                Project #5 Adding lighting
//                Name: Scott Schumacher
//                Intro to Computer Graphics 
//******************************************************************************
// added lighting initialization
// declared indices for light and material coefficients; getUniformLocationCalls;
//
//
//******************************************************************************

#include "Object.h"

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat angularSpeed = 5.0 * DegreesToRadians; // amount by which theta is incremented per click

//******************************************************************************

GLuint modelViewLoc; // Model-view matrix uniform shader variable location
mat4 model_view;
vec4 eye, at, up, ctr_box;  // locations of eye, focus of view; direction up.
GLfloat BBoxMax, z_eye = 4.0; // max dimension of bounding box; distance from model to camera

//*********************************************************************************************************************

GLuint ProjectionLoc; // projection matrix uniform shader variable location
mat4 projection; // projection matrix and assoc variables
GLfloat fovy = 45.0; // field of view, degrees, y- axis: start at 45
GLfloat zFar, zNear, aspectRatio; //near and far limits of clipping volume; ration w:h of viewport

//*********************************************************************************************************************
// Lighting initialization-copied from Angel ch5 example2.cpp

vec4 lightPos(5.0, 5.0, 5.0, 0.0); // a vector, not a position; all rays from this source will be parallel, as from 
// an infinitely distant source.
vec3 lightD(1.0, 1.0, 1.0);
vec3 lightS(1.0, 1.0, 1.0);
vec3 lightA(0.2, 0.2, 0.2);
GLuint lightPosLoc, lightDLoc, lightSLoc, lightALoc;

//*********************************************************************************************************************
// material initialization-copied from Angel ch5 example2.cpp
vec3 matlD(0.880000, 0.113923, 0.446084);
vec3 matlS(0.990000, 0.428872, 0.551990);
vec3 matlA(1.0, 1.0, 1.0);
float shininess = 300.0;
GLuint matlDLoc, matlSLoc, matlALoc, shinyLoc;

//*********************************************************************************************************************


//*********************************************************************************************************************
const int numModels =3 ;
Object instModels[numModels] = {Object("doodad.obj"), Object("jack.obj"), Object("cube.obj")/*, Object("lem.obj")*/}; 

int modelChoice = 0;  // active model

//*********************************************************************************************************************

GLuint vao[numModels]; // vertex array object for each model

//*********************************************************************************************************************

// OpenGL initialization
void
init()
{
   // Load shaders and use the resulting shader program
   GLuint program = InitShader("vshader42.glsl", "fshader42.glsl");
   glUseProgram(program);
   
   // Create a vertex array object for each model to be displayed
   glGenVertexArrays(numModels, vao); // SSM
   
   // loop-  bind vao for each file 
   for (int m = 0; m < numModels; m++)
   {
      glBindVertexArray(vao[m]);  
      instModels[m].load(program); //program returned by InitShader.cpp
   }

   // binding an already bound vao makes it the active object
   glBindVertexArray(vao[modelChoice]);
      
   // passing uniform variables to shader
   ProjectionLoc = glGetUniformLocation(program, "projection");  
   modelViewLoc = glGetUniformLocation(program, "model_view"); 

   lightPosLoc = glGetUniformLocation(program, "lightPos");
   glUniform4fv(lightPosLoc, 1, lightPos);
   
   lightDLoc = glGetUniformLocation(program, "lightD");
   glUniform3fv(lightDLoc, 1, lightD);
   lightSLoc = glGetUniformLocation(program, "lightS");
   glUniform3fv(lightSLoc, 1, lightS);
   lightALoc = glGetUniformLocation(program, "lightA");
   glUniform3fv(lightALoc, 1, lightA);

   matlDLoc = glGetUniformLocation(program, "matlD");
   glUniform3fv(matlDLoc, 1, matlD);
   matlSLoc = glGetUniformLocation(program, "matlS");
   glUniform3fv(matlSLoc, 1, matlS);
   matlALoc = glGetUniformLocation(program, "matlA");
   glUniform3fv(matlALoc, 1, matlA);
   shinyLoc = glGetUniformLocation(program, "shininess");
   glUniform1f(shinyLoc, shininess);
   
   glEnable(GL_DEPTH_TEST);
   glClearColor(1.0, 1.0, 1.0, 1.0);
}

//*********************************************************************************************************************

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

//*********************************************************************************************************************
int frame, fps, time, timebase = 0;

void
display(void)
{
   // model view is changeable so is declared right before draw
   //mat4 model_view; 
   // product of the thre axes rotation matrices
   mat4 Rotato = RotX(Theta[Xaxis]) * RotY(Theta[Yaxis]) * RotZ(Theta[Zaxis]); 

   // three elements required for LookAt, which becomes model_view
   eye = vec4(0.0, 0.0, z_eye, 1.0); 
   at = instModels[modelChoice].ctr_box(); 
   up = vec4(0.0, 3.0, 0.0, 0.0);  
   model_view = LookAt(eye, at, up); 

   // model view is multiplied by the rotation matrix
   model_view *= Rotato; 

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // send model_view to the shaders as a uniform variable
   glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view);
   
   instModels[modelChoice].draw(); // call to draw

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

//*********************************************************************************************************************

bool wirestate = true; // used for toggling wireframe view

void
keyboard(unsigned char key, int x, int y)
{
   switch (key) 
   {

   case 'l': fovy -= 5; break; // zooms in (increases "focal length")
   case 'L': fovy += 5; break; // zooms out (decreases "focal length")

   // reverse the direction of rotation
   case 'r': angularSpeed *= -1; break;

   // switch between models
   case 's': modelChoice = (modelChoice += 1) % numModels; 
      glBindVertexArray(vao[modelChoice]); z_eye = 4.0;
      Theta[Xaxis] = Theta[Yaxis] = Theta[Zaxis] = 0; fovy = 45;  break;

   case 'w': // toggle wireframe-- got from cynorfleet, masonellis
      glPolygonMode(GL_FRONT_AND_BACK, (wirestate) ? GL_LINE : GL_FILL);
      wirestate = !wirestate;
      break;
   
   // move the eye away from / closer to the model
   case 'Z': z_eye *= 1.1; break; 
   case 'z': z_eye *= 0.9; break; 
   
   case 033: // Escape Key
   case 'q': case 'Q':
      exit(EXIT_SUCCESS);
      break;
   }

   // projection is altered by changing the field of view
   BBoxMax = (instModels[modelChoice].bounding_box_max());
   projection = Perspective(fovy, aspectRatio, zNear, zFar); // SSZ
   glUniformMatrix4fv(ProjectionLoc, 1, GL_TRUE, projection); // SSZ
}

//*********************************************************************************************************************

void  // mouse buttons control rotation of model; 'r' key reverses direction
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

//*********************************************************************************************************************

void
idle(void)
{
   
   glutPostRedisplay();
}

//*********************************************************************************************************************

void // required to resize window / maintain aspect ratio
reshape(int width, int height)
{
   glViewport(0, 0, width, height);
   aspectRatio = GLfloat (width) / height;
   BBoxMax = (instModels[modelChoice].bounding_box_max());
   zFar = 30;
   zNear = 0.05;
   projection = Perspective(fovy, aspectRatio, zNear, zFar); // SSZ
   glUniformMatrix4fv(ProjectionLoc, 1, GL_TRUE, projection); // SSZ
}


//*********************************************************************************************************************

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
   glutIdleFunc(idle);
   glutReshapeFunc(reshape); //SS

   glutMainLoop();

   return 0;
}
