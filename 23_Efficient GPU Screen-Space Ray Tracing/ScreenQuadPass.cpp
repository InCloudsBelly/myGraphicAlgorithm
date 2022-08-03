#include "ScreenQuadPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>

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
	m_pShader->setTextureUniformValue("u_Texture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RayMarchingTexture"));
}

//************************************************************************************
//Function:
void _ScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}