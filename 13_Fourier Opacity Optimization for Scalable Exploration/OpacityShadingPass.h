#pragma once
#include "RenderPass.h"

class _Sponza;

class COpacityShadingPass : public _RenderPass
{
public:
	COpacityShadingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~COpacityShadingPass();

	virtual void initV();
	virtual void updateV();
private:
	int m_FBO = -1;
	std::shared_ptr<_Sponza> m_pSponza;
};