#pragma once
#include "RenderPass.h"

class _NonuniformBlurPass : public _RenderPass
{
public:
	_NonuniformBlurPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~_NonuniformBlurPass();

	virtual void initV() override;
	virtual void updateV() override;
private:
	GLint m_FBO;
	std::shared_ptr<MyGraphics::STexture> m_FirstTexture;
};