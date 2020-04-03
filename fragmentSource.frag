#version 330 core

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoord;

out vec4 FragColor;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec3 lightPos;
uniform vec3 lightDir;

void main()
{
	vec3 reflectVec = reflect(-lightDir, Normal);
	vec3 cameraVec = normalize(viewPos - FragPos);

	float spec = pow(max(dot(reflectVec, cameraVec), 0), material.shininess);
	vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * light.specular;

	vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * max(dot(lightDir, Normal), 0) * light.diffuse;

	vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * light.ambient;


	FragColor = vec4((ambient + diffuse + specular), 1.0);
    
} 
