#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>

struct SVPL
{
	glm::vec4 PositionInViewSpace;
	glm::vec3 NormalInViewSpace;
	glm::vec3 RadianceIntensity;
	float Alphai;
	float Radius;
};

class _CalculateVPLRadiusPass : public _RenderPass
{
public:
	_CalculateVPLRadiusPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_CalculateVPLRadiusPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	float m_MaxErrorBound = 0.0005f;

	void __gengrateRandomNumbers4VPLs();
};