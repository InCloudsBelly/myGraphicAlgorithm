#include "GBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

GBufferPass::GBufferPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

GBufferPass::~GBufferPass()
{
}

void GBufferPass::initV()
{
	
	MyGraphics::Camera::setMainCameraFarPlane(100);
	MyGraphics::Camera::setMainCameraMoveSpeed(1.0);
	MyGraphics::Camera::setMainCameraPos({ 0, -2.03285, 3.00298});
	MyGraphics::Camera::setMainCameraFront({ -0.305803, 0.190466, -0.932849 });
	//MyGraphics::Camera::setMainCameraFront({ 0, 0.176569, -0.983381 });

	m_pShader = std::make_shared<_Shader>("GBuffer_VS.glsl", "GBuffer_FS.glsl");

	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	auto TextureAlbedo = std::make_shared<MyGraphics::STexture>();
	auto TextureDepth = std::make_shared<MyGraphics::STexture>();
	TextureAlbedo->InternalFormat = GL_RGBA32F;
	TextureAlbedo->ExternalFormat = GL_RGBA;
	TextureAlbedo->DataType  = GL_FLOAT;
	genTexture(TextureAlbedo);
	TextureDepth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureDepth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureDepth->DataType = GL_FLOAT;
	TextureDepth->Type4MinFilter = GL_NEAREST;
	TextureDepth->Type4MagFilter = GL_NEAREST;
	TextureDepth->TextureAttachmentType = MyGraphics::STexture::_TextureAttachmentType::DepthTexture;
	genTexture(TextureDepth);

	m_FBO = genFBO({ TextureAlbedo,TextureDepth });

	MyGraphics::ResourceManager::registerSharedData("AlbedoTexture", TextureAlbedo);
	MyGraphics::ResourceManager::registerSharedData("DepthTexture", TextureDepth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Near", MyGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", MyGraphics::Camera::getMainCameraFar());
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(MyGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->initModel(*m_pShader);

}

void GBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(MyGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}