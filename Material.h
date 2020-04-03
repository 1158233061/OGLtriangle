#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
	Shader* shader;//着色器
	glm::vec3 diffuse;//漫反射
	glm::vec3 specular;//镜面反射
	glm::vec3 ambient;//环境光
	float shininess;//光照强度

	Material(Shader* _shader, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _ambient, float _shininess);
};