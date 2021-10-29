#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 Ka;
layout (location = 3) in vec3 Kd;
layout (location = 4) in vec3 Ks;
layout (location = 5) in float phongExp;
uniform float intensity;
uniform vec3 eye;
uniform vec3 lightDir;
uniform mat4 transformMatrix;
uniform mat4 modelMatrix;
out vec4 color;
void main()
{
   gl_Position = transformMatrix * modelMatrix * vec4(aPos, 1.0);
   vec4 vertexPosition = modelMatrix * vec4(aPos, 1.0);
   vec4 transformedNormal4 = modelMatrix * vec4(normal, 1.0);
   vec3 transformedNormal = normalize(vec3(transformedNormal4.x, transformedNormal4.y, transformedNormal4.z));
   vec3 ambient = Ka;
   vec3 diffuse = Kd * max(0, dot(lightDir, transformedNormal));
   vec4 eyeDir4 = vec4(eye, 1.0) - vertexPosition;
   vec3 eyeDir = normalize(vec3(eyeDir4.x, eyeDir4.y, eyeDir4.z));
   vec3 h = normalize(lightDir + eyeDir);
   vec3 specular;
   float specularIntensity = pow(max(0.0, dot(transformedNormal, h)), phongExp / 2.0);
   // pow operation can return Nan eg. pow(0, 0)
   if (isnan(specularIntensity)) {
      specular = vec3(0.0);
   }
   else {
      specular = Ks * specularIntensity;
   }
   color = vec4(ambient + diffuse + specular, 1);
}