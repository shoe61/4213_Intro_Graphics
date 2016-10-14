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
   int numNormals;
   vector<vec4>vertices;
   vector<vec4> normals;
   vector<GLuint> vertIndices;
   vector<GLuint> normIndices;
   vec4* colorsP; //expedient- points to vertIndices
 
   GLuint buffer, Ibuffer;
   
public:
   Object(char* file_name);
   ~Object();
   int load(GLuint);
   void draw();
};

