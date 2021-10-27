#version 330 core
in vec3 transformedNormal;
in vec3 Ka;
in vec3 Kd;
in vec3 Ks;
in float phongExp;
uniform float intensity;
uniform vec3 eyeDir;
uniform vec3 lightDir;
out vec4 FragColor;
void main()
{
   vec3 ambient = Ka;
   vec3 diffuse = Kd * max(0, dot(lightDir, transformedNormal));
   vec3 reflectDir = -lightDir + 2 * dot(lightDir, transformedNormal) * transformedNormal;
   vec3 specular = Ks * pow(max(0, dot(eyeDir, reflectDir)), phongExp);
   FragColor = vec4(ambient + diffuse, 1);
}

