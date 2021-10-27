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
   vec3 diffuse = Kd * max(0.0, dot(lightDir, transformedNormal));
   vec3 h = normalize(lightDir + transformedNormal);
   vec3 specular;
   float specularIntensity = pow(max(0.0, dot(transformedNormal, h)), phongExp);
   // pow operation can return Nan eg. pow(0, 0)
   if (isnan(specularIntensity)) {
      specular = vec3(0.0);
   }
   else {
      specular = Ks * pow(max(0.0, dot(transformedNormal, h)), phongExp);
   }
   FragColor = vec4(ambient + diffuse + specular, 1.0);
}

