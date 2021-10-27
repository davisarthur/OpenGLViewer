#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 Ka;
layout (location = 3) in vec3 Kd;
layout (location = 4) in vec3 Ks;
layout (location = 5) in float phongExp;
uniform float intensity;
uniform vec3 eyeDir;
uniform vec3 lightDir;
uniform mat4 transformMatrix;
uniform mat4 modelMatrix;
out vec4 color;
void main()
{
   gl_Position = transformMatrix * modelMatrix * vec4(aPos, 1.0);
   vec4 transformedNormal4 = transformMatrix * modelMatrix * vec4(normal, 1.0);
   vec3 transformedNormal = vec3(transformedNormal4.x, transformedNormal4.y, transformedNormal4.z);
   vec3 ambient = Ka;
   vec3 diffuse = Kd * max(0, dot(lightDir, transformedNormal));
   vec3 reflectDir = -lightDir + 2 * dot(lightDir, transformedNormal) * transformedNormal;
   vec3 specular = Ks * pow(max(0, dot(eyeDir, reflectDir)), phongExp);
   color = vec4(ambient + diffuse, 1);
}