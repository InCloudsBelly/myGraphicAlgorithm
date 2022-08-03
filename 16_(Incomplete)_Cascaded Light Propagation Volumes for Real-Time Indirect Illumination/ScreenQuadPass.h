#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class _ScreenQuadPass : public _RenderPass
{
public:
	_ScreenQuadPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~_ScreenQuadPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_OldKeyMStatusSave = -1;
	int m_FBO;
};