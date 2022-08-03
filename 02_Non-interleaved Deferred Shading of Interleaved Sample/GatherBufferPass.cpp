#include "GatherBufferPass.h"
#include "Shader.h"
#include "Utils.h"
#include "Interface.h"

_GatherBufferPass::_GatherBufferPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_GatherBufferPass::~_GatherBufferPass()
{
}

void _GatherBufferPass::initV()
{
	auto TextureConfig = std::make_shared<MyGraphics::STexture>();;
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_LINEAR;
	TextureConfig->Type4MagFilter = GL_LINEAR;
	TextureConfig->ImageBindUnit = 0;
	genTexture(TextureConfig);
	MyGraphics::ResourceManager::registerSharedData("GatherTexture", TextureConfig);

	auto ShadingTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("ShadingTexture");

	m_pShader = std::make_shared<_Shader>("GatherBufferPass_CS.glsl");

	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setIntUniformValue("u_SubBufferNumX", MyGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumX"));
	m_pShader->setIntUniformValue("u_SubBufferNumY", MyGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumY"));
	m_pShader->setTextureUniformValue("u_LightedSplitedTexture", ShadingTexture);
	m_pShader->setImageUniformValue(TextureConfig);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void _GatherBufferPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}