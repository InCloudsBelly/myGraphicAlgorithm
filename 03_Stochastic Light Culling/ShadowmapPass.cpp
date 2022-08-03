#include "ShadowmapPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_ShadowmapPass::_ShadowmapPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_ShadowmapPass::~_ShadowmapPass()
{
}

void _ShadowmapPass::initV()
{
	m_pShader = std::make_shared<_Shader>("Shadowmap_VS.glsl", "Shadowmap_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	auto TextureColor = std::make_shared<MyGraphics::STexture>();
	auto TextureDepth = std::make_shared<MyGraphics::STexture>();
	genTexture(TextureColor);
	TextureDepth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureDepth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureDepth->DataType = GL_FLOAT;
	TextureDepth->Type4MinFilter = GL_LINEAR;
	TextureDepth->Type4MagFilter = GL_LINEAR;
	TextureDepth->Width = TextureDepth->Height = m_ShadowmapResolution;
	TextureDepth->Type4WrapS = TextureDepth->Type4WrapT = GL_CLAMP_TO_BORDER;
	TextureDepth->BorderColor = { 0,0,0,0 };
	TextureDepth->TextureAttachmentType = MyGraphics::STexture::_TextureAttachmentType::DepthTexture;
	genTexture(TextureDepth);

	m_FBO = genFBO({ TextureColor, TextureDepth });

	MyGraphics::ResourceManager::registerSharedData("LightDepthTexture", TextureDepth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::mat4 LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pSponza->initModel(*m_pShader);
}

void _ShadowmapPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, m_ShadowmapResolution, m_ShadowmapResolution);

	m_pShader->activeShader();
#ifndef _DEBUG
	glm::mat4 LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
#endif
	m_pSponza->updateModel(*m_pShader);

	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}