#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class _IndirectShadingPass : public _RenderPass
{
public:
	_IndirectShadingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_IndirectShadingPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_FBO;
};