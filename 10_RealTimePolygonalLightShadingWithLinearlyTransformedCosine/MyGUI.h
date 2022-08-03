#pragma once
#include "GUI.h"
#include <GLM/glm.hpp>

class _MyGUI : public _GUI
{
public:
	_MyGUI(const std::string& vName, int vExcutionOrder);
	virtual ~_MyGUI();

	virtual void initV() override;
	virtual void updateV() override;

private:
	glm::vec4 m_DiffuseColor = glm::vec4(1);
	glm::vec4 m_SpecularColor = glm::vec4(1);
	glm::vec3 m_LightSourceRotation;
	float	  m_Roughness = 0.0f;
	float	  m_Intensity = 1.0f;
	bool	  m_IsTwoSide = false;
};