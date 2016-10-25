//******************************************************************************
//                Project #3 Rotating Objects
//                Name: Scott Schumacher
//                Intro to Computer Graphics 10/14/2016
//******************************************************************************
// This file, Object.cpp, contains the implementation of the Object class de-
// clared in Object.h. There are four public methods: a parameterized constructor
// which receives as its only parameter the name of the file to be instantiated;
// the Load method, which creates buffer objects on the GPU and initializes them;
// the Draw method, which directs the GPU to display the buffed data; and the 
// Destructor.

//----------------------------------------------------------------------------

#include "Object.h"

//----------------------------------------------------------------------------
// The constructor, Object::Object(char* file_name) accepts as an argument the
// file name of the model to be instantiated. It checks to make sure the file
// exists and is open. Then it inspects each line for vertex, normal, or index
// data, and appends that data to the appropriate vector. The constructor is 
// set up as a while loop, so it inspects every line; the only other loop is
// a for 3 loop in the index routine. The overall complexity of this method is
// O(n), where n = the number of vertices.

Object::Object(char* file_name)
{
   //open the file and check that it's open
   ifstream in;
   in.open(file_name);
   if (!in)
   {
      cerr << "Oops! Can't open " << file_name;
      exit(1);
   }
   name = file_name;

   string delim;           // delimiter is the string at the beginning of a line  
                           // thatindicates the type of data contained therein.
   
   double vx, vy, vz;      // vertex x, y, and z cooridinates
   double vnx, vny, vnz;   //normal x, y, and z coordinates  
   int vv, tt, nn;         //vertex, texture (not used), and normal indices

   stringstream ss;        // stringstream object created from each line, transfers  
                           // data to the appropriate vectors.
   string line;            // file is read line by line

   
   while (getline(in, line))              // Iterate through the file line by line
   {
      if (line.substr(0, 2) == "v ")      // Identify the vertex data lines
      {
         ss << line;                      // read the line out to a stringstream object
         ss >> delim >> vx >> vy >> vz;   // parse the delimiter and vertex coordinates
         ss.str(""); ss.clear();          // empty the stringstream object and clear flags
        
         vertices.push_back(vec4(vx, vy, vz, 1));        // store vertex data
       }
      numVertices = vertices.size();      // vertex count
      
      /*if (line.substr(0, 2) == "vn")      // repeat above procedure for vertex normals
      {
         ss << line;
         ss >> delim >> vnx >> vny >> vnz;
         ss.str(""); ss.clear();
         normals.push_back(vec4(vnx, vny, vnz, 1));
      }numNormals = normals.size(); */      


      if (line.substr(0, 2) == "f ")      // read indices; there are three sets per line
      {
         ss << line;
         ss >> delim;
         char cdelim;

         for (int i = 0; i < 3; i++)
         {
            ss >> vv >> cdelim >> cdelim >> nn;
            vertIndices.push_back((vv) -1);
            //normIndices.push_back((nn)-1);
         }
         ss.str(""); ss.clear();
      }
      numIndices = vertIndices.size();
      
   }

   // use the vertex indices to create an array of triangles
   for (int i = 0; i < numIndices; i++)
   {
      points.push_back(vertices[vertIndices[i]]);
   }
}

//----------------------------------------------------------------------------

int Object::load(GLuint program)
{
   // Create and initialize a buffer object
   
   int size = numIndices * 16;   // each vertex requires 16 bytes, and is listed repeatedly
                                 // each time it's used...
   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   
   //put color and vertex indices in same buffer
   glBufferData(GL_ARRAY_BUFFER, size + size, NULL, GL_STATIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, size, &points[0]);
   glBufferSubData(GL_ARRAY_BUFFER, size, size, &points[0]);

   //load the index buffer for the vertices
   /*glGenBuffers(1, &Ibuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * 4, &vertIndices[0], GL_STATIC_DRAW);*/
   
   GLuint vPosition = glGetAttribLocation(program, "vPosition");
   glEnableVertexAttribArray(vPosition);
   glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(0));   // offset 0: vertex position data in first half of buffer

   GLuint vColor = glGetAttribLocation(program, "vColor");
   glEnableVertexAttribArray(vColor);
   glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(size));   // offset size: color data in second half of buffer

   return 0;
}

//----------------------------------------------------------------------------

void Object::draw()
{
   //glBindVertexArray(buffer);
   //glBindVertexArray(Ibuffer);
   glDrawArrays(GL_TRIANGLES, 0, numIndices);
}

//----------------------------------------------------------------------------

Object::~Object()
{
}
