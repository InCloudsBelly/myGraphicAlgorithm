#include "RSMBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "HalfCornelBox.h"
#include "Lucy.h"
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
	m_pHalfCornellBox = std::dynamic_pointer_cast<_HalfCornellBox>(MyGraphics::ResourceManager::getGameObjectByName("HalfCornellBox"));
	m_Lucy = std::dynamic_pointer_cast<Lucy>(MyGraphics::ResourceManager::getGameObjectByName("Lucy"));

	auto TexturePosition = std::make_shared<MyGraphics::STexture>();
	auto TextureNormal = std::make_shared<MyGraphics::STexture>();
	auto TextureFlux = std::make_shared<MyGraphics::STexture>();
	TextureFlux->InternalFormat = TextureNormal->InternalFormat = TexturePosition->InternalFormat = GL_RGBA32F;
	TextureFlux->ExternalFormat = TextureNormal->ExternalFormat = TexturePosition->ExternalFormat = GL_RGBA;
	TextureFlux->DataType = TextureNormal->DataType = TexturePosition->DataType = GL_FLOAT;
	TextureFlux->Width = TextureNormal->Width = TexturePosition->Width = m_RSMResolution;
	TextureFlux->Height = TextureNormal->Height = TexturePosition->Height = m_RSMResolution;
	TextureFlux->Type4WrapS = TextureNormal->Type4WrapS = TexturePosition->Type4WrapS = GL_CLAMP_TO_BORDER;
	TextureFlux->Type4WrapT = TextureNormal->Type4WrapT = TexturePosition->Type4WrapT = GL_CLAMP_TO_BORDER;
	TextureFlux->BorderColor = TextureNormal->BorderColor = TexturePosition->BorderColor = { 0,0,0,0 };

	genTexture(TextureFlux);
	genTexture(TextureNormal);
	genTexture(TexturePosition);


	m_FBO = genFBO({ TextureFlux,TextureNormal,TexturePosition });

	MyGraphics::ResourceManager::registerSharedData("RSMFluxTexture", TextureFlux);
	MyGraphics::ResourceManager::registerSharedData("RSMNormalTexture", TextureNormal);
	MyGraphics::ResourceManager::registerSharedData("RSMPositionTexture", TexturePosition);

	m_pShader->activeShader();
	glm::vec3 LightPos = glm::vec3(0.9 , 0.7, 7);	
	glm::vec3 LightDir = glm::normalize(glm::vec3(-0.6,-0.7,-1.6));	
	glm::mat4 LightViewMatrix = glm::lookAt(LightPos, LightPos + LightDir, glm::vec3(0, 1, 0));
	glm::mat4 LightProjectionMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10.0f);
	glm::mat4 LightVPMatrix = LightProjectionMatrix * LightViewMatrix;
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightVPMatrix));

	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pHalfCornellBox->getModelMatrix()));
	m_pHalfCornellBox->initModel(*m_pShader);

	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_Lucy->getModelMatrix()));
	m_Lucy->initModel(*m_pShader);

	MyGraphics::ResourceManager::registerSharedData("LightVPMatrix", LightVPMatrix);
	MyGraphics::ResourceManager::registerSharedData("RSMResolution", m_RSMResolution);
	MyGraphics::ResourceManager::registerSharedData("LightDir", LightDir);
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
	
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_Lucy->getModelMatrix()));
	m_Lucy->updateModel(*m_pShader);

	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pHalfCornellBox->getModelMatrix()));
	m_pHalfCornellBox->updateModel(*m_pShader);

	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}