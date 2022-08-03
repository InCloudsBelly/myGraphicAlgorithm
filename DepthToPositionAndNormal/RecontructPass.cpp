#include "RecontructPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"

_RecontructPass::_RecontructPass(const std::string& vPassName, int vExecutionOrder) : _RenderPass(vPassName, vExecutionOrder)
{
}

_RecontructPass::~_RecontructPass()
{
}

//************************************************************************************
//Function:
void _RecontructPass::initV()
{
	m_pShader = std::make_shared<_Shader>("Recontruct_VS.glsl", "Recontruct_FS.glsl");
	auto Texture = std::make_shared<MyGraphics::STexture>();
	Texture->InternalFormat = GL_RGBA32F;
	Texture->ExternalFormat = GL_RGBA;
	Texture->DataType = GL_FLOAT;
	genTexture(Texture);


	m_FBO = genFBO({ Texture });

	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setFloatUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setFloatUniformValue("u_Near", MyGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Fov", glm::radians(MyGraphics::Camera::getMainCameraFov()));
	m_pShader->setFloatUniformValue("u_Far", MyGraphics::Camera::getMainCameraFar());
	m_pShader->setTextureUniformValue("u_DepthTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("DepthTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
}

//************************************************************************************
//Function:
void _RecontructPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}