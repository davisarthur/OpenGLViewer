#version 330 core
in vec3 vertexPosition;
in vec3 transformedNormal;
in vec3 Ka;
in vec3 Kd;
in vec3 Ks;
in float phongExp;
uniform float intensity;
uniform vec3 eye;
uniform vec3 lightDir;
out vec4 FragColor;
void main()
{
   vec3 normal = normalize(transformedNormal);
   vec3 ambient = Ka;
   vec3 diffuse = Kd * max(0.0, dot(lightDir, normal));
   vec3 eyeDir = normalize(eye - vertexPosition);
   vec3 reflectDir = reflect(lightDir, normal);
   vec3 specular;
   float specularIntensity = pow(max(0.0, dot(eyeDir, reflectDir)), phongExp);
   // pow operation can return Nan eg. pow(0, 0)
   if (isnan(specularIntensity)) {
      specular = vec3(0.0);
   }
   else {
      specular = Ks * specularIntensity;
   }
   FragColor = vec4(ambient + diffuse + specular, 1.0);
}

