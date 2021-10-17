#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transformMatrix;
void main()
{
   gl_Position = transformMatrix * vec4(aPos.x * 0.5, aPos.y * 1.5, aPos.z, 1.0);
}