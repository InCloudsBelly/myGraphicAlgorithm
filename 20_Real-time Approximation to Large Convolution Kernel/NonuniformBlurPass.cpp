#include "NonuniformBlurPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"

_NonuniformBlurPass::_NonuniformBlurPass(const std::string& vPassName, int vExecutionOrder) : _RenderPass(vPassName, vExecutionOrder)
{
}

_NonuniformBlurPass::~_NonuniformBlurPass()
{
}

//************************************************************************************
//Function:
void _NonuniformBlurPass::initV()
{
	m_pShader = std::make_shared<_Shader>("NonuniformBlur_VS.glsl", "NonuniformBlur_FS.glsl");
	m_FirstTexture = std::make_shared<MyGraphics::STexture>();
	m_FirstTexture->InternalFormat = GL_RGBA32F;
	m_FirstTexture->ExternalFormat = GL_RGBA;
	m_FirstTexture->DataType = GL_FLOAT;
	m_FirstTexture->isMipmap = true;
	genTexture(m_FirstTexture);

	m_FBO = genFBO({ m_FirstTexture });
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_CoarserTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));

	MyGraphics::ResourceManager::registerSharedData("Texture", m_FirstTexture);

}

//************************************************************************************
//Function:
float angle = 0;
void _NonuniformBlurPass::updateV()
{
	genGenerateMipmap(MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));
	float x = glm::cos(glm::radians(angle)) * 0.5;
	float y = glm::sin(glm::radians(angle)) * 0.5;
	int size = 1;
	for (int i = 10; i >= 0; i--)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_FirstTexture->TextureID, i);
		glViewport(0, 0, size, size);
		m_pShader->activeShader();
		m_pShader->setFloatUniformValue("g_focus", 0, 0);
		m_pShader->setIntUniformValue("g_level", i);
		m_pShader->setTextureUniformValue("u_SourceTexture", m_FirstTexture);
		m_pShader->setTextureUniformValue("u_CoarserTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));
		drawQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		size *= 2;
	}
	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	angle++;
}