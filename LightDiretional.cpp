#include "LightDiretional.h"
#include <glm/gtx/rotate_vector.hpp>

LightDiretional::LightDiretional(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color) :
	position(_position),
	angles(_angles),
	color(_color)
{
	UpdateDirection();
}

void LightDiretional::UpdateDirection()
{
	direction = glm::vec3(0, 0, 1.0f);
	direction = glm::rotateX(direction, angles.x);
	direction = glm::rotateY(direction, angles.y);
	direction = glm::rotateZ(direction, angles.z);
	direction = -direction;
}
