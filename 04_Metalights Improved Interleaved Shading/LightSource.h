#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>
#include <vector>
using namespace std;
struct _PointLight
{
	glm::vec4  Position;
	glm::vec4  ColorAndRadius;
	glm::vec4 Importance;
};

class _LightSources : public _GameObject
{
public:
	_LightSources(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~_LightSources();
	_LightSources();
	static _LightSources& get_instance() {
		static _LightSources instance;
		return instance;
	};
	virtual void initV() override;
	virtual void updateV() override;
	vector<_PointLight> *getPointLights();
private:
	std::vector<_PointLight> *m_LightSources;
	int m_LightNum = 5000;
};