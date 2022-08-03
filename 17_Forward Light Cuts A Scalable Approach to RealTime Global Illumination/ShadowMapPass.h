#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
class _Sponza;

class _ShadowMapPass : public _RenderPass
{
public:
	_ShadowMapPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_ShadowMapPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Sponza> m_pSponza;
	int m_FBO = -1;
	int m_RSMResolution = 512;
};