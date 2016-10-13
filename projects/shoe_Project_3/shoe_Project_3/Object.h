#include<vector>
#include "Angel.h"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include"mat.h"
#include"vec.h"


using namespace std;


#pragma once

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
   Object();
   Object(char* file_name);
   ~Object();
   int load(GLuint);
   void draw();
};

