//******************************************************************************
//                Project #3 Rotating Objects
//                Name: Scott Schumacher
//                Intro to Computer Graphics 10/14/2016
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
   //int numNormals;
   vector<vec4>vertices;
   //vector<vec4> normals;
   vector<GLuint> vertIndices;
   //vector<GLuint> normIndices;
   vec4* colorsP; //expedient- points to vertIndices

   vector<vec4>points;
   int numPoints;
 
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

