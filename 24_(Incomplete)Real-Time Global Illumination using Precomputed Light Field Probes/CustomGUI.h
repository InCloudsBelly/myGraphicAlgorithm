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
	glm::vec3 m_LightPos = glm::vec3(-0.15, -1.13, -0.58);	//��Ӧ1.0��LightCameraSize��ȥ�����Ļ�z=0.19
	glm::vec3 m_LightDir = glm::normalize(glm::vec3(-1.0, -0.7071, 0));	//-0.3, -1, 0
	float m_Intensity = 10.0f;

};