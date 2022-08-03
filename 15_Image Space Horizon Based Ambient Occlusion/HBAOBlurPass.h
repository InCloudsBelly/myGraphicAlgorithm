#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class _HBAOBlurPass : public _RenderPass
{
public:
	_HBAOBlurPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_HBAOBlurPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_FBO;
};