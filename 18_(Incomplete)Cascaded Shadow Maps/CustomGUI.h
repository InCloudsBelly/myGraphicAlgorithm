#pragma once
#include "GUI.h"
#include <GLM/glm.hpp>

class _CustomGUI : public _GUI
{
public:
	_CustomGUI(const std::string& vName, int vExcutionOrder);
	virtual ~_CustomGUI();

	virtual void initV() override;
	virtual void updateV() override;

private:

	glm::vec3 m_LightPos = glm::vec3(0, 0.304485, 4.43392);
	glm::vec3 m_LightDir = glm::normalize(glm::vec3(1, -0.5, -1));
	float m_Intensity = 10.0f;
};