#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Ka;
layout (location = 2) in vec3 Kd;
layout (location = 3) in vec3 Ks;
uniform mat4 transformMatrix;
uniform mat4 modelMatrix;
out vec4 color;
void main()
{
   gl_Position = transformMatrix * modelMatrix * vec4(aPos, 1.0);
   color = vec4(Kd, 1.0);
}