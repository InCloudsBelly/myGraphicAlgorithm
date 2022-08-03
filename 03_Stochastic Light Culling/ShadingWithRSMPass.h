#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>

class _ShadingWithRSMPass : public _RenderPass
{
public:
	_ShadingWithRSMPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_ShadingWithRSMPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	glm::mat4 m_LightViewMatrix;
	glm::mat4 m_LightProjectionMatrix;
	glm::mat4 m_LightVPMatrix;
	glm::vec3 m_LightDir;
	bool m_EnableIndirectIllumination = true;
	int  m_OldKeyIStatus = -1;
};