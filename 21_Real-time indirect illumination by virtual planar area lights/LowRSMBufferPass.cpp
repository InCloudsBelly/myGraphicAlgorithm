#include "LowRSMBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "AABB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CLowRSMBufferPass::CLowRSMBufferPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

CLowRSMBufferPass::~CLowRSMBufferPass()
{
}

void CLowRSMBufferPass::initV()
{
	m_pShader = std::make_shared<_Shader>("LowRSMBuffer_VS.glsl", "LowRSMBuffer_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));

	m_pDynamicObjectShader = std::make_shared<_Shader>("LowRSMBuffer_VS.glsl", "LowRSMBuffer_FS.glsl");
	m_pDynamicObject = std::dynamic_pointer_cast<_DynamicObject>(MyGraphics::ResourceManager::getGameObjectByName("DynamicObject"));;
	auto TexturePosition = std::make_shared<MyGraphics::STexture>();
	TexturePosition->InternalFormat = GL_RGBA32F;
	TexturePosition->ExternalFormat = GL_RGBA;
	TexturePosition->DataType = GL_FLOAT;
	TexturePosition->Width = m_Block;
	TexturePosition->Height = m_Block;
	TexturePosition->Type4WrapT = GL_CLAMP_TO_EDGE;
	TexturePosition->Type4WrapS = GL_CLAMP_TO_EDGE;
	TexturePosition->Type4MinFilter = GL_NEAREST_MIPMAP_NEAREST;
	TexturePosition->Type4MagFilter = GL_NEAREST;
	TexturePosition->isMipmap = true;
	genTexture(TexturePosition);

	m_FBO = genFBO({TexturePosition});

	MyGraphics::ResourceManager::registerSharedData("PlaneCentreTexture", TexturePosition);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::mat4 LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	m_pSponza->initModel(*m_pShader);
	m_pDynamicObjectShader->activeShader();
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pDynamicObject->getModelMatrix()));
	m_pDynamicObjectShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pDynamicObject->initModel(*m_pDynamicObjectShader);
	MyGraphics::ResourceManager::registerSharedData("Block", m_Block);
}

void CLowRSMBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, m_Block, m_Block);

	m_pShader->activeShader();
	glm::mat4 LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
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