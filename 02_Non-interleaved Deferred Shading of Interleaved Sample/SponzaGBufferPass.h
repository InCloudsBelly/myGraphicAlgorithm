#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class _Sponza;

class _SponzaGBufferPass : public _RenderPass
{
public:
	_SponzaGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_SponzaGBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<_Sponza> m_pSponza;
	GLint m_FBO;
};