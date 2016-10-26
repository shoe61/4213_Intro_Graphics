#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include<vector>
#include"Angel.h"
#include"mat.h"
#include"vec.h"

#pragma comment(lib, "freeglut")
#pragma comment(lib, "glew32")

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

using namespace std;


vector<vec4> vertices;

vector<vec4> normals;

int numVertices, numNormals, numIndices;

vector<unsigned int> vertIndices;
vector<unsigned int> normIndices;

double vx, vy, vz, vnx, vny, vnz;
string delim;



void main() // reads and parses the data in an object file
{
   char* file_name = "butcherBlock.obj";

   ifstream in;
   in.open(file_name);
   if (!in)
   {
      cerr << "Oops! Can't open " << file_name;
      exit(1);
   }

   stringstream ss;
   string line, s;
   int i = 0, j= 0, k = 0;
   int vv, tt, nn;

   while(getline(in, line))
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
            vertIndices.push_back((vv)-1); 
            normIndices.push_back((nn)-1); 
            cout << "vv: " << vv; cout << "nn: " << nn << endl;
         }
         
         ss.str(""); ss.clear();
         k++;
         

      }
      
   } 
   numVertices = i;
   cout << "numVertices: " << numVertices << endl;
   numNormals = j;
   cout << "numNormals: " << numNormals << endl;
   numIndices = k;
   cout << "numIndices: " << numIndices << endl;
}
