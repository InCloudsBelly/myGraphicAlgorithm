#include "SponzaGBufferPass.h"
#include "Shader.h"
#include "Sponza.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_SponzaGBufferPass::_SponzaGBufferPass(const std::string& vPassName, int vExcutionOrder) :_RenderPass(vPassName, vExcutionOrder)
{
}

_SponzaGBufferPass::~_SponzaGBufferPass()
{
}

//************************************************************************************
//Function:
void _SponzaGBufferPass::initV()
{
	m_pShader = std::make_shared<_Shader>("Sponza_VS.glsl", "Sponza_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
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
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pSponza->initModel(*m_pShader);
}

//************************************************************************************
//Function:
void _SponzaGBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();

	m_pSponza->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}