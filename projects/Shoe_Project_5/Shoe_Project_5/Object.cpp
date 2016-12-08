//*************************************************************************************************
//                Project #5 Model View and Perspective with Lighting and materials
//                Name: Scott Schumacher
//                Intro to Computer Graphics 11/27/2016
//*************************************************************************************************
// This file, Object.cpp, contains the implementation of the Object class de-
// clared in Object.h. There are four public methods from the previous iteration:
// a parameterized constructor which receives as its only parameter the name of 
// the file to be instantiated; the Load method, which creates buffer objects 
// on the GPU and initializes them; the Draw method, which directs the GPU to 
// display the buffered data; and the Destructor.
//  
// 

//-------------------------------------------------------------------------------------------------

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
   // get the name of the material file, which is the same as the name of the obj file with 
   // the ending .mtl  and strip the ending:
   string fileName = file_name;
   size_t lastIndex = fileName.find_last_of(".");
   string nakedName = fileName.substr(0, lastIndex);
   
   // and append the .mtl ending to the naked name
   string matlFile = nakedName + ".mtl";

   // open the material file and check that it's open
   ifstream matl;
   matl.open(matlFile);
   if (!matl)
   {
      cerr << "Oops! Can't open " << matlFile;
      exit(1);
   }
   
   //open the object file and check that it's open
   ifstream in;
   in.open(file_name);
   if (!in)
   {
      cerr << "Oops! Can't open " << file_name;
      exit(1);
   }
   
   string delim;           // delimiter is the string at the beginning of a line  
                           // thatindicates the type of data contained therein.
   
   double vx, vy, vz;      // vertex x, y, and z cooridinates
   double vnx, vny, vnz;   //normal x, y, and z coordinates  
   int vv, tt, nn;         //vertex, texture (not used), and normal indices


   
   stringstream ss;        // stringstream object created from each line
   string line;            // file is read line by line


   while (getline(in, line))              // Iterate through the file line by line
   {
      if (line.substr(0, 2) == "v ")      // Identify the vertex data lines
      {
         ss << line;                      // read the line out to a stringstream object
         ss >> delim >> vx >> vy >> vz;   // parse the delimiter and vertex coordinates

         // these are to determine the min and max values of vx, vy, and vz
         if (vx > maxvx) maxvx = vx; if (vx < minvx) minvx = vx;
         if (vy > maxvy) maxvy = vy; if (vy < minvy) minvy = vy;
         if (vz > maxvz) maxvz = vz; if (vz < minvz) minvz = vz;

         ss.str(""); ss.clear();          // empty the stringstream object and clear flags
         points.push_back(vec4(vx, vy, vz, 1));        // store vertex data
      }
           
      if (line.substr(0, 2) == "vn")      // repeat above procedure for vertex normals
      {
         ss << line;
         ss >> delim >> vnx >> vny >> vnz;
         ss.str(""); ss.clear();
         
         abNormals.push_back(vec4(vnx, vny, vnz, 0.0));
      }      


      if (line.substr(0, 2) == "f ")      // read indices; there are three sets per line
      {
         ss << line;
         ss >> delim;
         char cdelim;

         for (int i = 0; i < 3; i++)
         {
            ss >> vv >> cdelim >> cdelim >> nn;
            vertIndices.push_back((vv) -1);
            normIndices.push_back((nn)-1);
         }
         ss.str(""); ss.clear();
      }
      numIndices = vertIndices.size();
   }

   //close the object file
   in.close();

   // use the vertex indices to create an array of triangles; the new array is 
   // the one that will be loaded for drawing. Makes it possible to use glDrawArrays
   for (int i = 0; i < numIndices; i++)
   {
      vertices.push_back(points[vertIndices[i]]);
      normals.push_back(abNormals[normIndices[i]]);
   }

   numVertices = vertices.size();
   numNormals = normals.size();

   // now read the material properties
   while (getline(matl, line))
   {
      if (line.substr(0, 3) == "Ns ")      // Identify the shininess constant
      {
         ss << line;                      // read the line out to a stringstream object
         ss >> delim >> shininess;   // parse shininess
         ss.str(""); ss.clear();          // empty the stringstream object and clear flags
      }
      
      if (line.substr(0, 3) == "Ka ")      // Identify the ambient constant lines
      {
         ss << line;                      // read the line out to a stringstream object
         ss >> delim >> Kar >> Kag >> Kab;   // parse red, blue, and green components of vec3 matlA
         matlA = vec3(Kar, Kag, Kab);  //instantiate the vector
         ss.str(""); ss.clear();          // empty the stringstream object and clear flags
      }

      if (line.substr(0, 3) == "Kd ")      // Identify the diffuse constant lines
      {
         ss << line;                      // read the line out to a stringstream object
         ss >> delim >> Kdr >> Kdg >> Kdb;   // parse red, blue, and green components of vec3 matlD
         matlD = vec3(Kdr, Kdg, Kdb);  //instantiate the vector
         ss.str(""); ss.clear();          // empty the stringstream object and clear flags
      }

      if (line.substr(0, 3) == "Ks ")      // Identify the spectral constant lines
      {
         ss << line;                      // read the line out to a stringstream object
         ss >> delim >> Ksr >> Ksg >> Ksb;   // parse red, blue, and green components of vec3 matlS
         matlS = vec3(Ksr, Ksg, Ksb);  //instantiate the vector
         ss.str(""); ss.clear();          // empty the stringstream object and clear flags
      }
   }
}

//-------------------------------------------------------------------------------------------------

// computes bounding box with 20 percent padding:
vec3 Object::bounding_box()
{
   vec3 bounding_box((maxvx - minvx)* 1.2, (maxvy - minvy)* 1.2, (maxvz - minvz)* 1.2);
   return bounding_box;
}

//-------------------------------------------------------------------------------------------------

// computes major axis of bounding box:

double Object::bounding_box_max()
{
   double bounding_box_max = (maxvx - minvx);
   if ((maxvy - minvy) > bounding_box_max) bounding_box_max = (maxvy - minvy);
   if ((maxvz - minvz) > bounding_box_max) bounding_box_max = (maxvz - minvz);
   bounding_box_max *= 1.75; // has to be padded to accommodate a rotating cube on its diagonal!
   return bounding_box_max;
}

//-------------------------------------------------------------------------------------------------

// computes the center of the bounding box:

vec4 Object::ctr_box()
{
   vec4 ctr_box((maxvx + minvx) / 2, (maxvy + minvy) / 2, (maxvz + minvz) / 2, 1);
   return ctr_box;
}

//-------------------------------------------------------------------------------------------------

int Object::load(GLuint program)
{
   // Create and initialize a buffer object
   
   int size = numIndices * 16;   // each vertex requires 16 bytes, and is listed repeatedly
                                 // each time it's used...

   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   
   //put color and vertex indices in same buffer
   glBufferData(GL_ARRAY_BUFFER, size + size, NULL, GL_STATIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vertices[0]);
   glBufferSubData(GL_ARRAY_BUFFER, size, size, &normals[0]);

   GLuint vPosition = glGetAttribLocation(program, "vPosition");
   glEnableVertexAttribArray(vPosition);
   glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(0));   // offset 0: vertex position data in first half of buffer

   GLuint vNormal = glGetAttribLocation(program, "vNormal");
   glEnableVertexAttribArray(vNormal);
   glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(size));   // offset size: normal data in second half

   matlDLoc = glGetUniformLocation(program, "matlD");
   matlSLoc = glGetUniformLocation(program, "matlS");
   matlALoc = glGetUniformLocation(program, "matlA");
   shinyLoc = glGetUniformLocation(program, "shininess");

   return 0;
}

//-------------------------------------------------------------------------------------------------

void Object::draw()  // material constants!
{
   glUniform3fv(matlDLoc, 1, matlD);
   glUniform3fv(matlSLoc, 1, matlS);
   glUniform3fv(matlALoc, 1, matlA);
   glUniform1f(shinyLoc, shininess);
   glDrawArrays(GL_TRIANGLES, 0, numIndices);
}

//-------------------------------------------------------------------------------------------------

Object::~Object()
{
}
