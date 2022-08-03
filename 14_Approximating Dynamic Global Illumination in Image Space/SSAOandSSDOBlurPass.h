#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class _SSAOandSSDOBlurPass : public _RenderPass
{
public:
	_SSAOandSSDOBlurPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_SSAOandSSDOBlurPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_FBO;
};