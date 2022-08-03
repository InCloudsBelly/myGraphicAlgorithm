#include "SplitGBufferPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"

_SplitGBufferPass::_SplitGBufferPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_SplitGBufferPass::~_SplitGBufferPass()
{
}

void _SplitGBufferPass::initV()
{
	auto AlbedoTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture");
	auto NormalTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture");
	auto PositionTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture");

	auto TextureAlbedo = std::make_shared<MyGraphics::STexture>();
	auto TextureNormal = std::make_shared<MyGraphics::STexture>(); 
	auto TexturePosition = std::make_shared<MyGraphics::STexture>();
	TextureAlbedo->InternalFormat = TextureNormal->InternalFormat = TexturePosition->InternalFormat = GL_RGBA32F;
	TextureAlbedo->ExternalFormat = TextureNormal->ExternalFormat = TexturePosition->ExternalFormat = GL_RGBA;
	TextureAlbedo->DataType = TextureNormal->DataType = TexturePosition->DataType = GL_FLOAT;
	TextureAlbedo->Type4MinFilter = TextureNormal->Type4MinFilter = TexturePosition->Type4MinFilter = GL_LINEAR;
	TextureAlbedo->Type4MagFilter = TextureNormal->Type4MagFilter = TexturePosition->Type4MagFilter = GL_LINEAR;
	TextureAlbedo->ImageBindUnit= 0;
	TextureNormal->ImageBindUnit = 1;
	TexturePosition->ImageBindUnit = 2;
	genTexture(TextureAlbedo);
	genTexture(TextureNormal);
	genTexture(TexturePosition);

	m_pShader = std::make_shared<_Shader>("SplitGBuffer_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputAlbedoTexture", AlbedoTexture);
	m_pShader->setTextureUniformValue("u_InputNormalTexture", NormalTexture);
	m_pShader->setTextureUniformValue("u_InputPositionTexture", PositionTexture);
	m_pShader->setIntUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setIntUniformValue("u_SubBufferNumX", m_SubBufferNumX);
	m_pShader->setIntUniformValue("u_SubBufferNumY", m_SubBufferNumY);
	m_pShader->setImageUniformValue(TextureAlbedo);
	m_pShader->setImageUniformValue(TextureNormal);
	m_pShader->setImageUniformValue(TexturePosition);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);

	MyGraphics::ResourceManager::registerSharedData("SubBufferNumX", m_SubBufferNumX);
	MyGraphics::ResourceManager::registerSharedData("SubBufferNumY", m_SubBufferNumY);
	MyGraphics::ResourceManager::registerSharedData("SplitedAlbedoImage", TextureAlbedo);
	MyGraphics::ResourceManager::registerSharedData("SplitedNormalImage", TextureNormal);
	MyGraphics::ResourceManager::registerSharedData("SplitedPositionImage", TexturePosition);
}

void _SplitGBufferPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}