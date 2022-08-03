#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class _Sponza;

class _DepthPass : public _RenderPass
{
public:
	_DepthPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_DepthPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<_Sponza> m_pSponza;
	GLint m_FBO;
};