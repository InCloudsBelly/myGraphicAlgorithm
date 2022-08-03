#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class _LightCamera;
class _DirectLightPass : public _RenderPass
{
public:
	_DirectLightPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_DirectLightPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<_LightCamera> m_pCLightCamera;
	std::vector<int> m_GlobalGroupSize;
	glm::mat4 m_LightVPMatrix;
	glm::vec3 m_LightDir;
};