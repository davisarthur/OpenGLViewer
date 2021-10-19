#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transformMatrix;
uniform float scaleFactor;
void main()
{
   gl_Position = transformMatrix * vec4(aPos.x * scaleFactor, aPos.y * scaleFactor, aPos.z * scaleFactor, 1.0);
}