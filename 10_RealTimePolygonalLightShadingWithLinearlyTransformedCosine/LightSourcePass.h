#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class _LightSourcePass : public _RenderPass
{
public:
	_LightSourcePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_LightSourcePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	float m_Intensity = 0.0;
};