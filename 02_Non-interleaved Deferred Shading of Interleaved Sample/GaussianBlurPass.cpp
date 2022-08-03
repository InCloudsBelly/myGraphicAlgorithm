#include "GaussianBlurPass.h"
#include "Common.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"

_GaussianBlurPass::_GaussianBlurPass(const std::string& vPassName, int vExecutionOrder) : _RenderPass(vPassName, vExecutionOrder)
{
}

_GaussianBlurPass::~_GaussianBlurPass()
{
}

void _GaussianBlurPass::initV()
{
	m_TextureConfig1 = std::make_shared<MyGraphics::STexture>();
	m_TextureConfig2 = std::make_shared<MyGraphics::STexture>();
	m_TextureConfig2->InternalFormat = m_TextureConfig1->InternalFormat = GL_RGBA32F;
	m_TextureConfig2->ExternalFormat = m_TextureConfig1->ExternalFormat = GL_RGBA;
	m_TextureConfig2->DataType = m_TextureConfig1->DataType = GL_FLOAT;
	m_TextureConfig2->Type4MinFilter = m_TextureConfig1->Type4MinFilter = GL_LINEAR;
	m_TextureConfig2->Type4MagFilter = m_TextureConfig1->Type4MagFilter = GL_LINEAR;
	m_TextureConfig1->ImageBindUnit = 0;
	m_TextureConfig2->ImageBindUnit = 0;
	genTexture(m_TextureConfig1);
	genTexture(m_TextureConfig2);
	MyGraphics::ResourceManager::registerSharedData("FinalTexture", m_TextureConfig2);

	m_pShader = std::make_shared<_Shader>("GaussianBlur_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setTextureUniformValue("u_InputTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("GatherTexture"));
	m_pShader->setTextureUniformValue("u_EdgeTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("EdgeTexture"));
	m_pShader->setImageUniformValue(m_TextureConfig1);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);

	m_IsBlurVerticalUniformId = m_pShader->getUniformId("u_IsBlurVertical");
	m_BlurInputTextureUniformId = m_pShader->getUniformId("u_InputTexture");
}

void _GaussianBlurPass::updateV()
{
	for (int i = 0; i < m_Iterations; ++i)
	{
		m_pShader->activeShader();
		m_pShader->setIntUniformValue(m_IsBlurVerticalUniformId, true);
		auto BindTexture4VerticalBlur = (i == 0) ? MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("GatherTexture") : m_TextureConfig2;
		m_pShader->setTextureUniformValue("u_InputTexture", BindTexture4VerticalBlur);
		m_pShader->setImageUniformValue(m_TextureConfig1);
		glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		m_pShader->activeShader();
		m_pShader->setIntUniformValue(m_IsBlurVerticalUniformId, false);
		m_pShader->setTextureUniformValue("u_InputTexture", m_TextureConfig1);
		m_pShader->setImageUniformValue(m_TextureConfig2);
		glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
}