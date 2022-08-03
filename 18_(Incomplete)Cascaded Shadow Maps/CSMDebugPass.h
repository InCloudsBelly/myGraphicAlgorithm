#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class _LightCamera;
class _CSMDebugPass : public _RenderPass
{
public:
	_CSMDebugPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_CSMDebugPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<_LightCamera> m_pCLightCamera;
	std::vector<int> m_GlobalGroupSize;
	glm::mat4 m_LightVPMatrix;
	glm::vec3 m_LightDir;
};