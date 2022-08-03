#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class _Sponza;

class DepthPeellingPass : public _RenderPass
{
public:
	DepthPeellingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~DepthPeellingPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Shader> m_BlendShader;
	std::shared_ptr<_Sponza> m_pSponza;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureColor;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureDepth;
	std::shared_ptr<MyGraphics::STexture> m_TextureConfig;
	std::vector<int> m_FBO;
	int m_BlendFBO = -1;
	int m_Layer = 4;
};