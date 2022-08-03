#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class _Sponza;

class _RSMBufferPass : public _RenderPass
{
public:
	_RSMBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_RSMBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Sponza> m_pSponza;
	int m_FBO = -1;
	int m_RSMResolution = 16;	//1024
	int m_ShadowmapResolution = 1024;
};