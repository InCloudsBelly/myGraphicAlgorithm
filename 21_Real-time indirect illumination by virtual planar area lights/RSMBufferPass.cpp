#include "RSMBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "AABB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CRSMBufferPass::CRSMBufferPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

CRSMBufferPass::~CRSMBufferPass()
{
}

void CRSMBufferPass::initV()
{
	m_pShader = std::make_shared<_Shader>("RSMBuffer_VS.glsl", "RSMBuffer_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pDynamicObjectShader = std::make_shared<_Shader>("RSMBufferDynamicObject_VS.glsl", "RSMBufferDynamicObject_FS.glsl");
	m_pDynamicObject = std::dynamic_pointer_cast<_DynamicObject>(MyGraphics::ResourceManager::getGameObjectByName("DynamicObject"));
	auto TexturePosition = std::make_shared<MyGraphics::STexture>();
	auto TextureNormalAndDoubleRoughness = std::make_shared<MyGraphics::STexture>();
	auto TextureRadiantFlux = std::make_shared<MyGraphics::STexture>();
	auto TextureAlbedoAndMetallic = std::make_shared<MyGraphics::STexture>();
	auto TextureDepth = std::make_shared<MyGraphics::STexture>(); ;
	TextureAlbedoAndMetallic->InternalFormat = TextureRadiantFlux->InternalFormat = TextureNormalAndDoubleRoughness->InternalFormat = TexturePosition->InternalFormat = GL_RGBA32F;
	TextureAlbedoAndMetallic->ExternalFormat = TextureRadiantFlux->ExternalFormat = TextureNormalAndDoubleRoughness->ExternalFormat = TexturePosition->ExternalFormat = GL_RGBA;
	TextureAlbedoAndMetallic->DataType = TextureRadiantFlux->DataType = TextureNormalAndDoubleRoughness->DataType = TexturePosition->DataType = GL_FLOAT;
	TextureAlbedoAndMetallic->Width = TextureRadiantFlux->Width = TextureNormalAndDoubleRoughness->Width = TexturePosition->Width = m_RSMResolution;
	TextureAlbedoAndMetallic->Height = TextureRadiantFlux->Height = TextureNormalAndDoubleRoughness->Height = TexturePosition->Height = m_RSMResolution;
	TextureAlbedoAndMetallic->Type4WrapS = TextureRadiantFlux->Type4WrapS = TextureNormalAndDoubleRoughness->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureAlbedoAndMetallic->Type4WrapT = TextureRadiantFlux->Type4WrapT = TextureNormalAndDoubleRoughness->Type4WrapT = GL_CLAMP_TO_EDGE;
	TextureDepth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureDepth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureDepth->DataType = GL_FLOAT;
	TextureDepth->Width = TextureDepth->Height = m_RSMResolution;
	TextureDepth->BorderColor = { 0,0,0,0 };
	TextureDepth->TextureAttachmentType = MyGraphics::STexture::_TextureAttachmentType::DepthTexture;

	genTexture(TextureRadiantFlux);
	genTexture(TextureNormalAndDoubleRoughness);
	genTexture(TexturePosition);
	genTexture(TextureAlbedoAndMetallic);
	genTexture(TextureDepth);
	m_FBO = genFBO({ TextureRadiantFlux,TextureNormalAndDoubleRoughness,TexturePosition, TextureAlbedoAndMetallic,TextureDepth });

	MyGraphics::ResourceManager::registerSharedData("RSMRadiantFluxTexture", TextureRadiantFlux);
	MyGraphics::ResourceManager::registerSharedData("RSMNormalAndDoubleRoughnessTexture", TextureNormalAndDoubleRoughness);
	MyGraphics::ResourceManager::registerSharedData("RSMPositionTexture", TexturePosition);
	MyGraphics::ResourceManager::registerSharedData("RSMAlbedoAndMetallicTexture", TextureAlbedoAndMetallic);
	MyGraphics::ResourceManager::registerSharedData("LightDepthTexture", TextureDepth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::mat4 LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	m_pShader->setIntUniformValue("u_VPLsCount", m_RSMResolution * m_RSMResolution);
	m_pShader->setFloatUniformValue("u_RSMCameraAreaInWorldSpace", MyGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace"));
	m_pShader->setFloatUniformValue("u_Intensity", MyGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_pSponza->initModel(*m_pShader);


	m_pDynamicObjectShader->activeShader();
	//m_pDynamicObjectShader->setTextureUniformValue("u_Texture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("InputTexture"));
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pDynamicObject->getModelMatrix()));
	m_pDynamicObjectShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pDynamicObjectShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	m_pDynamicObjectShader->setIntUniformValue("u_VPLsCount", m_RSMResolution*m_RSMResolution);
	m_pDynamicObjectShader->setFloatUniformValue("u_RSMCameraAreaInWorldSpace", MyGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace"));
	m_pDynamicObjectShader->setFloatUniformValue("u_Intensity", MyGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_pDynamicObject->initModel(*m_pDynamicObjectShader);
	MyGraphics::ResourceManager::registerSharedData("RSMResolution", m_RSMResolution);
}

void CRSMBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, m_RSMResolution, m_RSMResolution);

	m_pShader->activeShader();
	glm::mat4 LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pSponza->updateModel(*m_pShader);

	m_pDynamicObjectShader->activeShader();
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pDynamicObject->getModelMatrix()));
	m_pDynamicObjectShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pDynamicObject->updateModel(*m_pDynamicObjectShader);

	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}