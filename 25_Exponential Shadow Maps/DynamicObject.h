#pragma once
#include "GameObject.h"

class _DynamicObject : public _GameObject
{
public:
	_DynamicObject(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~_DynamicObject();

	virtual void initV() override;
	virtual void updateV() override;
	glm::vec3 pos =  glm::vec3(0, 0, 0);
	glm::vec3 pos1 = glm::vec3(1.2, -0.8,  1.6);
	glm::vec3 pos2 = glm::vec3(1.2, -0.8, -4.4);

	float time = 2;
};
