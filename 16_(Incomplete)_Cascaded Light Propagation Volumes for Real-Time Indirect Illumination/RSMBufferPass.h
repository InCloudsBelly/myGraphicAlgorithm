#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>

class _Sponza;

class CRSMBufferPass : public _RenderPass
{
public:
	CRSMBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CRSMBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Sponza> m_pSponza;
	int m_FBO = -1;
	int m_RSMResolution = 128;	
};