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

//point4 vertices[8];
vector<point4> vertices;

//point4 normals[6];
vector<point4> normals;

vector<unsigned int> vertIndices;
vector<unsigned int> normIndices;

void main() // reads and parses the data in an object file
{
   //open the file
   ifstream in;
   in.open("butcherBlock.obj");
   string delim;
   double vx, vy, vz;
   double vn1, vn2, vn3;
   int numVertices;
   int numNormals;
   int numIndices;
   
   //read the file, ignoring everything until beginning of vertex list
   in >> delim;
   while (delim != "v")
   {
      in >> delim;
      cout << delim;//DIAGNOREMO
   }

   cout << "delim : " << delim << endl;
   //read the vertex list until reading "vn", which denotes the start of the vertex normal list
   int i = 0;
   while (delim == "v")
   {
      in >> vx >> vy >> vz >> delim;
      
      //cout << "\nvx" << i << ": " << vx << "\nvy" << i << ": " << vy << "\nvz" << i << " : " << vz << endl << endl;//DIAGNOREMO
      vertices.push_back(point4(vx, vy, vz, 1.0));
      cout << "vertices[" << i << "]: " << vertices[i] << endl;//DIAGNOREMO
      //in >> delim; 
      //cout << "vertex delim, in loop: " << delim << endl;//DIAGNOREMO
      i++; 
   } 
   numVertices = i;
   cout << "numVertices: " << numVertices << endl;





   //cout << "before starting normals, delim = " << delim << endl; //DIAGNOREMO


   //read the vertex normals as long as delim == "vn"
   int j = 0; //use as index for normals array
   while (delim == "vn")
   {
      in >> vn1 >> vn2 >> vn3 >> delim;
      //cout << "\nvn1" << j << ": " << vn1 << "\nvn2" << j << ": " << vn2 << "\nvn3" << j << " : " << vn3 << endl << endl;//DIAGNOREMO
      normals.push_back(point4(vn1, vn2, vn3, 0));
      cout << "normals[" << j << "]: " << normals[j] << endl;
      j++;
   } 
   numNormals = j;
   cout << "numNormals: " << numNormals << endl;

   //cout << "at end, delim = " << delim << endl; //DIAGNOREMO

   while (delim != "f"){ in >> delim;}

   //cout << "now, delim is: " << delim << endl;//DIAGNOREMO


   // read in the face info
   char cdelim = NULL; //delimiter is now a character rather than a string
   int k = 0;
   int vv, tt, nn;
   while (cdelim != '/')
   {
      for (int i = 0; i < 3; i++) //repeat the read sequence 3 times per line
      {
         in >> vv;
         vertIndices.push_back(vv); cout << "vv: " << vv;
         in >> cdelim; 
         //cout << "in face loop, cdelim = " << cdelim;//DIAGNOREMO

         if (cdelim == '/') //check to see if there's a tt value, or another '/'
         {
            in >> cdelim;
            //cout << "in the if, cdelim = " << cdelim << endl;//DIAGNOREMO
            if (cdelim == '/'){ tt = NULL; }//two '/'s indicates no texture index
            else{ in >> tt; in >> cdelim; } // but there might be! if so, read it and the following cdelim.
         }

         in >> nn; 
         cout << "\tnn: " << nn << endl;//DIAGNOREMO
         normIndices.push_back(nn);
      }
      in >> cdelim; 
      //cout << "cdelim after 3loop: " << cdelim << endl;//DIAGNOREMO
      //if (cdelim == '/'){ cout << "eof" << endl; }//DIAGNOREMO
      k++;
   }
   numIndices = k;
   cout << "numIndices: " << numIndices << endl;
}
