#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>
#include <vector>

struct _PointLight
{
	glm::vec4  Position;
	glm::vec4  ColorAndRadius;
};

class _LightSources : public _GameObject
{
public:
	_LightSources(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~_LightSources();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<_PointLight> m_LightSources;
	int m_LightNum = 5000;
};