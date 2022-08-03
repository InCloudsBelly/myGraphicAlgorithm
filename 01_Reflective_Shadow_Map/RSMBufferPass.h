#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class _HalfCornellBox;
class Lucy;


class _RSMBufferPass : public _RenderPass
{
public:
	_RSMBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_RSMBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_HalfCornellBox> m_pHalfCornellBox;
	std::shared_ptr<Lucy> m_Lucy;
	int m_FBO = -1;
	int m_RSMResolution = 256;	//1024
	glm::vec3 m_DirectionalLightDirection = glm::vec3(-1, -1, -1);
};