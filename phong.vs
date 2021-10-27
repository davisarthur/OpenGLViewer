#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 KaIn;
layout (location = 3) in vec3 KdIn;
layout (location = 4) in vec3 KsIn;
layout (location = 5) in float phongExpIn;
uniform mat4 transformMatrix;
uniform mat4 modelMatrix;
out vec3 transformedNormal;
out vec3 Ka;
out vec3 Kd;
out vec3 Ks;
out float phongExp;
void main()
{
   gl_Position = transformMatrix * modelMatrix * vec4(aPos, 1.0);
   vec4 transformedNormal4 = modelMatrix * vec4(normal, 1.0);
   transformedNormal = vec3(transformedNormal4.x, transformedNormal4.y, transformedNormal4.z);
   Ka = KaIn;
   Kd = KdIn;
   Ks = KsIn;
   phongExp = phongExpIn;
}