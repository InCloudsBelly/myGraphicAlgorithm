#include "RSMBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "AABB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_RSMBufferPass::_RSMBufferPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_RSMBufferPass::~_RSMBufferPass()
{
}

void _RSMBufferPass::initV()
{
	m_pShader = std::make_shared<_Shader>("RSMBuffer_VS.glsl", "RSMBuffer_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	auto TexturePosition = std::make_shared<MyGraphics::STexture>();
	auto TextureNormal = std::make_shared<MyGraphics::STexture>();
	auto TextureRandianceIntensity = std::make_shared<MyGraphics::STexture>();
	TextureRandianceIntensity->InternalFormat = TextureNormal->InternalFormat = TexturePosition->InternalFormat = GL_RGBA32F;
	TextureRandianceIntensity->ExternalFormat = TextureNormal->ExternalFormat = TexturePosition->ExternalFormat = GL_RGBA;
	TextureRandianceIntensity->DataType = TextureNormal->DataType = TexturePosition->DataType = GL_FLOAT;
	TextureRandianceIntensity->Width = TextureNormal->Width = TexturePosition->Width = m_RSMResolution;
	TextureRandianceIntensity->Height = TextureNormal->Height = TexturePosition->Height = m_RSMResolution;
	TextureRandianceIntensity->Type4WrapS = TextureNormal->Type4WrapS = TexturePosition->Type4WrapS = GL_CLAMP_TO_BORDER;
	TextureRandianceIntensity->Type4WrapT = TextureNormal->Type4WrapT = TexturePosition->Type4WrapT = GL_CLAMP_TO_BORDER;
	TextureRandianceIntensity->BorderColor = TextureNormal->BorderColor = TexturePosition->BorderColor = { 0,0,0,0 };
	genTexture(TextureRandianceIntensity);
	genTexture(TextureNormal);
	genTexture(TexturePosition);

	m_FBO = genFBO({ TextureRandianceIntensity,TextureNormal,TexturePosition });

	MyGraphics::ResourceManager::registerSharedData("RSMRandianceIntensityTexture", TextureRandianceIntensity);
	MyGraphics::ResourceManager::registerSharedData("RSMNormalTexture", TextureNormal);
	MyGraphics::ResourceManager::registerSharedData("RSMPositionTexture", TexturePosition);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::mat4 LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	m_pShader->setIntUniformValue("u_VPLsCount", m_RSMResolution*m_RSMResolution);
	m_pShader->setFloatUniformValue("u_RSMCameraAreaInWorldSpace", MyGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace"));

	m_pSponza->initModel(*m_pShader);

	MyGraphics::ResourceManager::registerSharedData("RSMResolution", m_RSMResolution);
}

void _RSMBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, m_RSMResolution, m_RSMResolution);

	m_pShader->activeShader();
#ifdef _DEBUG
	glm::mat4 LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
#endif
	m_pSponza->updateModel(*m_pShader);

	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}