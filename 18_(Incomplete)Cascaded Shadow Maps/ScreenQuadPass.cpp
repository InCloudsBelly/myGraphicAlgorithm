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
	m_pShader->setTextureUniformValue("u_Texture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("DirectIlluminationImage"));
}

//************************************************************************************
//Function:
void _ScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_O) == GLFW_PRESS && m_OldKeyDebug != GLFW_PRESS)
	{
		m_OldKeyDebug = GLFW_PRESS;
		m_pShader->setTextureUniformValue("u_Texture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("CSMDebugImage"));
		std::cout << "Debug" << std::endl;
	}
	else if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_O) == GLFW_RELEASE)
		m_OldKeyDebug = GLFW_RELEASE;
	if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_PRESS && m_OldKeyNormal != GLFW_PRESS)
	{
		m_OldKeyNormal = GLFW_PRESS;
		m_pShader->setTextureUniformValue("u_Texture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("DirectIlluminationImage"));

		std::cout << "Normal " << std::endl;
	}
	else if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_RELEASE)
		m_OldKeyNormal = GLFW_RELEASE;
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}