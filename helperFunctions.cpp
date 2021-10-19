// reading a text file
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include "helperFunctions.h"
using namespace std;

SceneObject::SceneObject(string fname, glm::vec3 worldPosIn, glm::vec3 scaleIn, glm::vec3 eulerAnglesIn) {
   triangles = readVertexData(fname);
   numBytes = triangles.size() * sizeof(triangles[0]);
   vertexSize = sizeof(triangles[0].vertex1);
   worldPos = worldPosIn;
   scale = scaleIn;
   eulerAngles = eulerAnglesIn;
   buildModelMat();
}

void SceneObject::updateModelMat(glm::vec3 deltaWorldPos, glm::vec3 deltaScale, glm::vec3 deltaEulerAngles) {
   worldPos += deltaWorldPos;
   scale += deltaScale;
   eulerAngles += deltaEulerAngles;
   buildModelMat();
}

void SceneObject::buildModelMat() {
   glm::mat4 scaleMat = glm::mat4(1.0);
   scaleMat[0] *= scale.x;
   scaleMat[1] *= scale.y;
   scaleMat[2] *= scale.z;
   modelMat = scaleMat;
}

vector<Triangle> readVertexData(string filename) {
   string line;
   ifstream myfile(filename);
   vector<vertexData> vertices;
   vector<Triangle> triangles;
   if (myfile.is_open()) {
      while (getline(myfile, line)) {
         vector<string> tokens = split(line, ' ');
         if (tokens.size() == 0) {
            continue;
         }
         else if (tokens[0] == "v") {
            float x = std::stof(tokens[1]);
            float y = std::stof(tokens[2]);
            float z = std::stof(tokens[3]);
            struct vertexData vertex;
            vertex.pos = glm::vec3(x, y, z);
            vertices.push_back(vertex);
         }
         else if (tokens[0] == "f") {
            vector<int> indeces;

            // read in vertex indeces of face
            for (int i = 1; i < tokens.size(); i++) {
               indeces.push_back(std::stoi(split(tokens[i], '/')[0]) - 1);
            } 

            // create first triangle
            Triangle t1;
            t1.vertex1 = vertices[indeces[0]];
            t1.vertex2 = vertices[indeces[1]];
            t1.vertex3 = vertices[indeces[2]];
            triangles.push_back(t1);

            // create any other triangles in that face
            for (int i = 0; i < tokens.size() - 4; i++) {
               Triangle t;
               t.vertex1 = vertices[indeces[0]];
               t.vertex2 = vertices[indeces[i + 2]];
               t.vertex3 = vertices[indeces[i + 3]];
               triangles.push_back(t);
            }
         }
      }
      myfile.close();
   }

   else cout << "Unable to open file"; 

   return triangles;
}

// String tokenizer based on the tutorial from https://www.geeksforgeeks.org/how-to-split-a-string-in-cc-python-and-java/
vector<string> split(string str, char del) {
   vector<string> tokens;
   // declaring temp string to store the curr "word" upto del
   string temp = "";

   for(int i=0; i<(int)str.size(); i++){
      // If cur char is not del, then append it to the cur "word", otherwise
      // you have completed the word, print it, and start a new word.
      if(str[i] != del) {
         temp += str[i];
      }
      else {
         if (temp.size() > 0) {
            tokens.push_back(temp);
         }
         temp = "";
      }
   }

   if (temp.size() > 0) {
      tokens.push_back(temp);
   }

   return tokens;
}

string readFile(string fileName) {
   string output = "";
   string line;
   ifstream myfile (fileName);
   if (myfile.is_open()) {
      while (getline(myfile, line)) {
         output += line + "\n";
      }
      myfile.close();
   }

   else cout << "Unable to open file";
   
   return output;
}