#pragma once
#include "RenderPass.h"

class _Sponza;

class _SponzaGBufferPass : public _RenderPass
{
public:
	_SponzaGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_SponzaGBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Sponza> m_pSponza;
	int m_FBO;
};