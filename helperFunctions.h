#include <glm/glm.hpp>
#include <vector>
#include <string>
using namespace std;

struct vertexData {
   glm::vec3 pos;
};

struct Triangle {
   struct vertexData vertex1;
   struct vertexData vertex2;
   struct vertexData vertex3;
};

string readFile(string fileName);

vector<Triangle> readVertexData(string fileName);

vector<string> split(string str, char del);