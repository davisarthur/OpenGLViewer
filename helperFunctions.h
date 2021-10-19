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

class SceneObject {
   public:
      vector<Triangle> triangles;
      int numBytes;
      int vertexSize;
      glm::vec3 scale;
      glm::vec3 worldPos;
      glm::vec3 eulerAngles;
      glm::mat4 modelMat;

      SceneObject(string fname, glm::vec3 worldPosIn, glm::vec3 scaleIn, glm::vec3 eulerAnglesIn);
      void updateModelMat(glm::vec3 deltaWorldPos, glm::vec3 deltaScale, glm::vec3 deltaEulerAngles);
   
   private:
      void buildModelMat();
};

string readFile(string fileName);

vector<Triangle> readVertexData(string fileName);

vector<string> split(string str, char del);