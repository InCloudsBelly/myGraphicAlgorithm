#pragma once
#include "RenderPass.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>

class _GroundPass : public _RenderPass
{
public:
	_GroundPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_GroundPass();

	virtual void initV() override;
	virtual void updateV() override;

private:

	glm::vec4 m_DiffuseColor;
	glm::vec4 m_SpecularColor;
	std::vector<glm::vec3> m_PolygonalLightVertexPos;
	float  m_Roughness = 0.0f;
	float  m_Intensity = 0.0f;
	bool   m_IsTwoSide = false;
	int m_OldKeyRightStatus = -1;
	int m_Layer = 0;
};