#include "Object.h"


Object::Object()
{
}
Object::Object(char* file_name)
{
   //open the file
   ifstream in;
   in.open(file_name);
   if (!in)
   {
      cerr << "Oops! Can't open " << file_name;
      exit(1);
   }
   name = file_name;

   string delim; // delimiter
   double vx, vy, vz; // vertex x, y, and z cooridinates
   double vnx, vny, vnz;  

   stringstream ss;
   string line, s;
   int i = 0, j = 0, k = 0;
   int vv, tt, nn;

   while (getline(in, line))
   {
      //cout << "line: " << line << endl;

      if (line.substr(0, 2) == "v ")
      {

         ss << line;
         //s = ss.str();
         //cout << s << endl;
         ss >> delim >> vx >> vy >> vz;
         ss.str(""); ss.clear();
         vertices.push_back(vec4(vx, vy, vz, 1));

         cout << "vertices[" << i << "]: " << vertices[i] << endl;
         i++;
      }


      if (line.substr(0, 2) == "vn")
      {

         ss << line;
         //s = ss.str();
         //cout << s << endl;
         ss >> delim >> vnx >> vny >> vnz;
         ss.str(""); ss.clear();
         normals.push_back(vec4(vnx, vny, vnz, 1));

         cout << "normals[" << j << "]: " << normals[j] << endl;
         j++;
      }
      if (line.substr(0, 2) == "f ")
      {
         ss << line;
         cout << line << endl;
         ss >> delim;

         char cdelim;

         for (int i = 0; i < 3; i++)
         {
            ss >> vv >> cdelim >> cdelim >> nn;
            vertIndices.push_back((vv) -1);
            normIndices.push_back((nn)-1);
            cout << "vv: " << vv; cout << "  nn: " << nn << endl;
         }

         ss.str(""); ss.clear();
         k++;


      }

   }
   numVertices = i;
   cout << "numVertices: " << numVertices << endl;
   numNormals = j;
   cout << "numNormals: " << numNormals << endl;
   numIndices = vertIndices.size();
   cout << "numIndices: " <<numIndices<< endl;
}



int Object::load(GLuint program)
{
   // Create and initialize a buffer object
   //GLuint buffer;
   int size = numVertices * 16;
   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   //put color and verts in same buffer
   glBufferData(GL_ARRAY_BUFFER, size + size, NULL, GL_STATIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vertices[0]);
   glBufferSubData(GL_ARRAY_BUFFER, size, size, &vertices[0]);

   //load the index buffer for the vertices
   glGenBuffers(1, &Ibuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * 4, &vertIndices[0], GL_STATIC_DRAW);

   GLuint vPosition = glGetAttribLocation(program, "vPosition");
   glEnableVertexAttribArray(vPosition);
   glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(0));

   GLuint vColor = glGetAttribLocation(program, "vColor");
   glEnableVertexAttribArray(vColor);
   glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(size));

   return 0;
}

void Object::draw()
{
   glBindVertexArray(buffer);
   glBindVertexArray(Ibuffer);
   glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

Object::~Object()
{
}
