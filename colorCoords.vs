#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transformMatrix;
out vec3 color;
void main()
{
   gl_Position = transformMatrix * vec4(aPos, 1.0);
   color = normalize(vec3(abs(gl_Position.x), abs(gl_Position.y), abs(gl_Position.z)));
}