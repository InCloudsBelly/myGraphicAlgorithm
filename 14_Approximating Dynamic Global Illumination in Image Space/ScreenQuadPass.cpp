#include "ScreenQuadPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"

_ScreenQuadPass::_ScreenQuadPass(const std::string& vPassName, int vExecutionOrder) : _RenderPass(vPassName, vExecutionOrder)
{
}

_ScreenQuadPass::~_ScreenQuadPass()
{
}

//************************************************************************************
//Function:
void _ScreenQuadPass::initV()
{
	m_pShader = std::make_shared<_Shader>("ScreenQuad_VS.glsl", "ScreenQuad_FS.glsl");

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_Albedo", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));
	m_pShader->setTextureUniformValue("u_SSAOTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("BlurSSAOTexture"));
	m_pShader->setTextureUniformValue("u_SSDOTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("BlurSSDOTexture"));
}

//************************************************************************************
//Function:
void _ScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}