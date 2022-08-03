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
	glm::vec3 m_LightPos = glm::vec3(0.0, 0.5, -0.6);	
	glm::vec3 m_LightDir = glm::normalize(glm::vec3(-1, -0.4, -0.1));
};