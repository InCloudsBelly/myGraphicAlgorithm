#pragma once
#include "RenderPass.h"
#include "Common.h"
#include <vector>

class _GaussianBlurPass : public _RenderPass
{
public:
	_GaussianBlurPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~_GaussianBlurPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int  m_Iterations = 1;
	int  m_IsBlurVerticalUniformId = -1;
	int  m_BlurInputTextureUniformId = -1;
	std::vector<int> m_GlobalGroupSize;
	std::shared_ptr<MyGraphics::STexture> m_TextureConfig1;
	std::shared_ptr<MyGraphics::STexture> m_TextureConfig2;
};