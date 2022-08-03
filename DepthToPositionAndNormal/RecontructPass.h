#pragma once
#include "RenderPass.h"

class _RecontructPass : public _RenderPass
{
public:
	_RecontructPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~_RecontructPass();

	virtual void initV() override;
	virtual void updateV() override;
private:
	GLint m_FBO;
};