#include "GBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "HalfCornelBox.h"
#include "Lucy.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_GBufferPass::_GBufferPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_GBufferPass::~_GBufferPass()
{
}

void _GBufferPass::initV()
{
	m_pShader = std::make_shared<_Shader>("HalfCornellBox_VS.glsl", "HalfCornellBox_FS.glsl");
	m_pHalfCornellBox = std::dynamic_pointer_cast<_HalfCornellBox>(MyGraphics::ResourceManager::getGameObjectByName("HalfCornellBox"));
	m_Lucy = std::dynamic_pointer_cast<Lucy>(MyGraphics::ResourceManager::getGameObjectByName("Lucy"));

	auto TexturePosition = std::make_shared<MyGraphics::STexture>();
	auto TextureNormal = std::make_shared<MyGraphics::STexture>();
	auto TextureAlbedo = std::make_shared<MyGraphics::STexture>();
	auto TextureDepth = std::make_shared<MyGraphics::STexture>();
	TextureAlbedo->InternalFormat = TextureNormal->InternalFormat = TexturePosition->InternalFormat = GL_RGBA32F;
	TextureAlbedo->ExternalFormat = TextureNormal->ExternalFormat = TexturePosition->ExternalFormat = GL_RGBA;
	TextureAlbedo->DataType = TextureNormal->DataType = TexturePosition->DataType = GL_FLOAT;
	genTexture(TextureAlbedo);
	genTexture(TextureNormal);
	genTexture(TexturePosition);
	TextureDepth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureDepth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureDepth->DataType = GL_FLOAT;
	TextureDepth->Type4MinFilter = GL_NEAREST;
	TextureDepth->Type4MagFilter = GL_NEAREST;
	TextureDepth->TextureAttachmentType = MyGraphics::STexture::_TextureAttachmentType::DepthTexture;
	genTexture(TextureDepth);

	m_FBO = genFBO({ TextureAlbedo,TextureNormal,TexturePosition,TextureDepth });

	MyGraphics::ResourceManager::registerSharedData("AlbedoTexture", TextureAlbedo);
	MyGraphics::ResourceManager::registerSharedData("NormalTexture", TextureNormal);
	MyGraphics::ResourceManager::registerSharedData("PositionTexture", TexturePosition);
	MyGraphics::ResourceManager::registerSharedData("DepthTexture", TextureDepth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pHalfCornellBox->getModelMatrix()));
	m_pHalfCornellBox->initModel(*m_pShader);

	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_Lucy->getModelMatrix()));
	m_Lucy->initModel(*m_pShader);

}

void _GBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_Lucy->getModelMatrix()));
	m_Lucy->updateModel(*m_pShader);

	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pHalfCornellBox->getModelMatrix()));
	m_pHalfCornellBox->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}