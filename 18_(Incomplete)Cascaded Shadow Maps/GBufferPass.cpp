#include "GBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "LightCamera.h"
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
	auto m_pCLightCamera = std::dynamic_pointer_cast<_LightCamera>(MyGraphics::ResourceManager::getGameObjectByName("LightCamera"));

	m_pShader = std::make_shared<_Shader>("GBuffer_VS.glsl", "GBuffer_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	auto TexturePosition = std::make_shared<MyGraphics::STexture>();
	auto TextureNormalAndDoubleRoughness = std::make_shared<MyGraphics::STexture>();
	auto TextureAlbedoAndMetallic = std::make_shared<MyGraphics::STexture>();
	TextureAlbedoAndMetallic->InternalFormat = TextureNormalAndDoubleRoughness->InternalFormat = TexturePosition->InternalFormat = GL_RGBA32F;
	TextureAlbedoAndMetallic->ExternalFormat = TextureNormalAndDoubleRoughness->ExternalFormat = TexturePosition->ExternalFormat = GL_RGBA;
	TextureAlbedoAndMetallic->DataType = TextureNormalAndDoubleRoughness->DataType = TexturePosition->DataType = GL_FLOAT;
	TextureAlbedoAndMetallic->isMipmap = TextureNormalAndDoubleRoughness->isMipmap = TexturePosition->isMipmap = false;
	genTexture(TextureAlbedoAndMetallic);
	genTexture(TextureNormalAndDoubleRoughness);
	genTexture(TexturePosition);
	m_FBO = genFBO({ TextureAlbedoAndMetallic, TextureNormalAndDoubleRoughness, TexturePosition});

	MyGraphics::ResourceManager::registerSharedData("AlbedoTexture", TextureAlbedoAndMetallic);
	MyGraphics::ResourceManager::registerSharedData("NormalTexture", TextureNormalAndDoubleRoughness);
	MyGraphics::ResourceManager::registerSharedData("PositionTexture", TexturePosition);
	
	std::vector<glm::mat4> Frustum = m_pCLightCamera->getFrustum();

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Near", MyGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", MyGraphics::Camera::getMainCameraFar());
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(Frustum[3] * MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix")));
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(MyGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->initModel(*m_pShader);
}

void GBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(MyGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}