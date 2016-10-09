#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include<vector>


using namespace std;

void main() // reads and parses the data in an object file
{
   
   string line, s;
   ifstream infile("cube.txt");

 
   stringstream t;
   const int BUFFSIZE = 80;
   char buff[BUFFSIZE];
   char buff2[BUFFSIZE];
   int j = 0;

   //traverses infile by line and reads character at a time into t
   if (infile.is_open())
   {
      while (getline(infile, line))
      {
         //j++; //this is to count the lines

         for (int i = 0; i < line.size(); i++)
         {
            char c = line[i];
            t << c;
         }

         const char *cstr = (t.str()).c_str();
         //const char *cstr = "v 0.232013 - 0.232013 - 0.232013";


         float a, b, c;
        
         s = t.str();

         t.str("");
         //cout << "s: " << s << endl;
                

         size_t v = s.find("v");
         cout << "v: " << v << endl;

         if (v == 0)
         {
            scanf_s(cstr, "%d %d %d\n", &a, &b, &c);

            printf("verts:  %d, %d, %d\n", a, b, c);

         }
         else{ continue;}



         

         
      }

     

   }
}
