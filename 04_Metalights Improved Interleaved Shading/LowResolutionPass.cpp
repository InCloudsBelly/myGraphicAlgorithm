#include "LowResolutionPass.h"
#include "Interface.h"
#include "Utils.h"
#include "LightSource.h"
#include "Shader.h"

_LowResolutionPass::_LowResolutionPass(const std::string& vPassName, int vExcutionOrder , MyGraphics::ERenderPassType vtype) : _RenderPass(vPassName, vExcutionOrder, vtype)
{
}

_LowResolutionPass::~_LowResolutionPass()
{
}

void _LowResolutionPass::initV()
{
	m_LowWidth = MyGraphics::WINDOW_KEYWORD::getWindowWidth() / 16;
	m_LowHeight = MyGraphics::WINDOW_KEYWORD::getWindowHeight() / 16;
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
	TextureAlbedo->ImageBindUnit = 0;
	TextureNormal->ImageBindUnit = 1;
	TexturePosition->ImageBindUnit = 2;
	TextureNormal->Width = TexturePosition->Width = TextureAlbedo->Width = m_LowWidth;
	TextureNormal->Height =TexturePosition->Height = TextureAlbedo->Height = m_LowHeight;
	genTexture(TextureAlbedo);
	genTexture(TextureNormal);
	genTexture(TexturePosition);

	m_pShader = std::make_shared<_Shader>("LowResolution_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setIntUniformValue("LowWidth", m_LowWidth);
	m_pShader->setIntUniformValue("LowHeight", m_LowHeight);
	m_pShader->setTextureUniformValue("u_InputAlbedoTexture", AlbedoTexture);
	m_pShader->setTextureUniformValue("u_InputNormalTexture", NormalTexture);
	m_pShader->setTextureUniformValue("u_InputPositionTexture", PositionTexture);
	m_pShader->setImageUniformValue(TextureAlbedo);
	m_pShader->setImageUniformValue(TextureNormal);
	m_pShader->setImageUniformValue(TexturePosition);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);

	MyGraphics::ResourceManager::registerSharedData("LowWidth", m_LowWidth);
	MyGraphics::ResourceManager::registerSharedData("LowHeight", m_LowHeight);
	MyGraphics::ResourceManager::registerSharedData("LowAlbedoImage", TextureAlbedo);
	MyGraphics::ResourceManager::registerSharedData("LowNormalImage", TextureNormal);
	MyGraphics::ResourceManager::registerSharedData("LowPositionImage", TexturePosition);
}

void _LowResolutionPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}