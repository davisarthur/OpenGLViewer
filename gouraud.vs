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
   vec3 transformedNormal = normalize(mat3(modelMatrix) * normal);
   vec3 ambient = Ka;
   vec3 diffuse = Kd * max(0, dot(lightDir, transformedNormal));
   vec3 eyeDir = normalize(eye - vec3(vertexPosition));
   vec3 reflectDir = reflect(lightDir, transformedNormal);
   vec3 specular;
   float specularIntensity = pow(max(0.0, dot(eyeDir, reflectDir)), phongExp);
   // pow operation can return Nan eg. pow(0, 0)
   if (isnan(specularIntensity)) {
      specular = vec3(0.0);
   }
   else {
      specular = Ks * specularIntensity;
   }
   color = vec4(ambient + diffuse + specular, 1);
}