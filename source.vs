#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transformMatrix;
uniform float scaleFactor;
void main()
{
   gl_Position = transformMatrix * scaleFactor * vec4(aPos, 1.0);
}