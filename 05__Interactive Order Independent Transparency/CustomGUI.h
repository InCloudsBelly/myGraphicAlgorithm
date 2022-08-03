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
	float m_NormalThresholdInEdgeDetecion = 0.5f;
	float m_DepthThresholdInEdgeDetection = 8.0f;
	//glm::vec3 m_LightPos = glm::vec3(0.52, -0.49, 0);	//对应0.25的LightCameraSize
	glm::vec3 m_LightPos = glm::vec3(-0.53, -0.5, 0.70);	//对应1.0的LightCameraSize，去除红光的话z=0.19
	glm::vec3 m_LightDir = glm::normalize(glm::vec3(-1.0, -1.0, 0));	//-0.3, -1, 0
};