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
	std::vector<glm::vec4> m_VPLsSampleCoordsAndWeights;
	int m_VPLNum = 256;
	float m_MaxSampleRadius = 50;
	glm::mat4 m_LightVPMatrix;
	glm::vec4 m_LightDir;

	void __initVPLsSampleCoordsAndWeights();
};