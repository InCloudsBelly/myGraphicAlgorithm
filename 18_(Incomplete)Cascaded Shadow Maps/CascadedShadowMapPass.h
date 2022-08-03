#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
class _Sponza;
class _LightCamera;
class CascadedShadowMapPass : public _RenderPass
{
public:
	CascadedShadowMapPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CascadedShadowMapPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Sponza> m_pSponza;
	std::shared_ptr<_LightCamera> m_pCLightCamera;
	int m_FBO = -1;
	int m_RSMResolution = 1024;
	int m_SplitNum = 8;
};