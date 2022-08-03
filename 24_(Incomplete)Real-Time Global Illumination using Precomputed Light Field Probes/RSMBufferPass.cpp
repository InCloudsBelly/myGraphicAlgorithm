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
	m_pDynamicObjectShader = std::make_shared<_Shader>("RSMBuffer_VS.glsl", "RSMBuffer_FS.glsl");
	m_pDynamicObject = std::dynamic_pointer_cast<_DynamicObject>(MyGraphics::ResourceManager::getGameObjectByName("DynamicObject"));
	auto TextureDepth = std::make_shared<MyGraphics::STexture>(); ;
	TextureDepth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureDepth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureDepth->DataType = GL_FLOAT;
	TextureDepth->Width = TextureDepth->Height = m_RSMResolution;
	TextureDepth->BorderColor = { 0,0,0,0 };
	TextureDepth->TextureAttachmentType = MyGraphics::STexture::_TextureAttachmentType::DepthTexture;

	genTexture(TextureDepth);
	m_FBO = genFBO({TextureDepth });

	;
	MyGraphics::ResourceManager::registerSharedData("LightDepthTexture", TextureDepth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pSponza->initModel(*m_pShader);


	m_pDynamicObjectShader->activeShader();
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pDynamicObject->getModelMatrix()));
	m_pDynamicObject->initModel(*m_pDynamicObjectShader);
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