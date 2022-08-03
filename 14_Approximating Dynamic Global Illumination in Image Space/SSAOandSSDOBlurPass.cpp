#include "SSAOandSSDOBlurPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <random>

_SSAOandSSDOBlurPass::_SSAOandSSDOBlurPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_SSAOandSSDOBlurPass::~_SSAOandSSDOBlurPass()
{
}

void _SSAOandSSDOBlurPass::initV()
{
	m_pShader = std::make_shared<_Shader>("SSAOandSSDOBlurPass_VS.glsl", "SSAOandSSDOBlurPass_FS.glsl");
	auto TextureFinial_AO = std::make_shared<MyGraphics::STexture>();
	TextureFinial_AO->InternalFormat = GL_RED;
	TextureFinial_AO->ExternalFormat = GL_RGB;
	TextureFinial_AO->DataType = GL_FLOAT;
	TextureFinial_AO->Width = MyGraphics::WINDOW_KEYWORD::getWindowWidth();
	TextureFinial_AO->Height = MyGraphics::WINDOW_KEYWORD::getWindowHeight();;
	TextureFinial_AO->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureFinial_AO->Type4WrapT = GL_CLAMP_TO_EDGE;
	genTexture(TextureFinial_AO);

	auto TextureFinial_DO = std::make_shared<MyGraphics::STexture>();
	TextureFinial_DO->InternalFormat = GL_RGBA32F;
	TextureFinial_DO->ExternalFormat = GL_RGBA;
	TextureFinial_DO->DataType = GL_FLOAT;
	TextureFinial_DO->Width = MyGraphics::WINDOW_KEYWORD::getWindowWidth();
	TextureFinial_DO->Height = MyGraphics::WINDOW_KEYWORD::getWindowHeight();;
	TextureFinial_DO->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureFinial_DO->Type4WrapT = GL_CLAMP_TO_EDGE;
	genTexture(TextureFinial_DO);

	m_FBO = genFBO({TextureFinial_AO,TextureFinial_DO });

	MyGraphics::ResourceManager::registerSharedData("BlurSSAOTexture", TextureFinial_AO);
	MyGraphics::ResourceManager::registerSharedData("BlurSSDOTexture", TextureFinial_DO);

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_SSAOTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("SSAOTexture"));
	m_pShader->setTextureUniformValue("u_SSDOTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("SSDOTexture"));

}

void _SSAOandSSDOBlurPass::updateV()
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
