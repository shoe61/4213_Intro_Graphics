//******************************************************************************
//                Project #4 Model View and Perspective with Rotating Objects
//                Name: Scott Schumacher
//                Intro to Computer Graphics 11/11/2016
//******************************************************************************
// This file, Object.cpp, is the declaration of the object class; it describes
// the variables, data structures, and methods that are common to objects of this
// class. I probably should have named it something other than "Object."

#pragma comment(lib, "freeglut")
#pragma comment(lib, "glew32")
#pragma once

#include<vector>
#include "Angel.h"
#include<string>
#include<fstream>
#include<sstream>

using namespace std;

class Object
{
   string name;
   int numVertices;
   int numIndices;
   int numNormals;
   vector<vec4>vertices;
   vector<vec4> normals;
   vector<GLuint> vertIndices;
   vector<GLuint> normIndices;
   vec4* colorsP; //expedient- points to vertIndices
   vector<vec3> obo; // name for a buffer to hold the material constants

   vec3 matlD, matlS, matlA;  // material constants, read from .mtl file
   float Kdr, Kdg, Kdb, Ksr, Ksg, Ksb, Kar, Kag, Kab;    // r, g, b components of vec3s matlD, matlS, matlA
   float shininess;           // shininess, also from .mtl file
   GLuint matlDLoc, matlSLoc, matlALoc, shinyLoc;  // locations in shader program of these variables

   vector<vec4>points; // this is the new vector initially containing the new vertices
   vector<vec4>abNormals; // this is the temp vector containing the new normals
   int numPoints;  // the new variable numbering thepoints
 
   GLuint buffer, Ibuffer;

   // variables used to find bounding box dimensions and center
   double maxvx = 0; double minvx = 0;
   double maxvy = 0; double minvy = 0;
   double maxvz = 0; double minvz = 0;
   
   
public:
   Object(char* file_name);
   ~Object();
   int load(GLuint);
   void draw();
   vec3 bounding_box();
   double bounding_box_max();
   vec4 ctr_box();
};

