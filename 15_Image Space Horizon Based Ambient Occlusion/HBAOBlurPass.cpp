#include "HBAOBlurPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <random>

_HBAOBlurPass::_HBAOBlurPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_HBAOBlurPass::~_HBAOBlurPass()
{
}

void _HBAOBlurPass::initV()
{
	m_pShader = std::make_shared<_Shader>("HBAOBlurPass_VS.glsl", "HBAOBlurPass_FS.glsl");
	auto TextureFinial = std::make_shared<MyGraphics::STexture>();
	TextureFinial->InternalFormat = GL_RED;
	TextureFinial->ExternalFormat = GL_RGB;
	TextureFinial->DataType = GL_FLOAT;
	TextureFinial->Width = MyGraphics::WINDOW_KEYWORD::getWindowWidth();
	TextureFinial->Height = MyGraphics::WINDOW_KEYWORD::getWindowHeight();;
	TextureFinial->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureFinial->Type4WrapT = GL_CLAMP_TO_EDGE;
	genTexture(TextureFinial);

	m_FBO = genFBO({TextureFinial});

	MyGraphics::ResourceManager::registerSharedData("BlurHBAOTexture", TextureFinial);

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_HBAOTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("HBAOTexture"));

}

void _HBAOBlurPass::updateV()
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
