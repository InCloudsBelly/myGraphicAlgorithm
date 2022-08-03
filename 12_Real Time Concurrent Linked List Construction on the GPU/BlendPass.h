#pragma once
#include "RenderPass.h"

class _BlendPass : public _RenderPass
{
public:
	_BlendPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_BlendPass();

	virtual void initV();
	virtual void updateV();
private:
	GLint m_FBO;
};