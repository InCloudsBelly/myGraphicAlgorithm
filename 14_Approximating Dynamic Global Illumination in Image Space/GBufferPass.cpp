#include "GBufferPass.h"
#include "Shader.h"
#include "CornellBox.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_GBufferPass::_GBufferPass(const std::string& vPassName, int vExcutionOrder) :_RenderPass(vPassName, vExcutionOrder)
{
}

_GBufferPass::~_GBufferPass()
{
}

//************************************************************************************
//Function:
void _GBufferPass::initV()
{
	MyGraphics::Camera::setMainCameraFarPlane(1000);
	MyGraphics::Camera::setMainCameraMoveSpeed(1.0);
	MyGraphics::Camera::setMainCameraPos({ -25.1728, 12.768, 80.1058});
	MyGraphics::Camera::setMainCameraFront({ 0.488694, -0.406737, -0.771844 });
	m_pShader = std::make_shared<_Shader>("GbufferPass_VS.glsl", "GbufferPass_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CornellBox>(MyGraphics::ResourceManager::getGameObjectByName("CornellBox"));
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
	m_pShader->setFloatUniformValue("u_Near", MyGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", MyGraphics::Camera::getMainCameraFar());
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pSponza->initModel(*m_pShader);
}

//************************************************************************************
//Function:
void _GBufferPass::updateV()
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