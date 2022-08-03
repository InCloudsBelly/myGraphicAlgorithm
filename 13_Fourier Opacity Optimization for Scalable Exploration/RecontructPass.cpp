#include "RecontructPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "Dragon.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CRecontructPass::CRecontructPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

CRecontructPass::~CRecontructPass()
{
}

void CRecontructPass::initV()
{


	auto TextureTansparent = std::make_shared<MyGraphics::STexture>();
	TextureTansparent->InternalFormat = GL_RGBA32F;
	TextureTansparent->ExternalFormat = GL_RGBA;
	TextureTansparent->DataType  = GL_FLOAT;
	genTexture(TextureTansparent);

	m_FBO = genFBO({ TextureTansparent });

	m_pShader = std::make_shared<_Shader>("RecontructPass_VS.glsl", "RecontructPass_FS.glsl");
	m_pDragon = std::dynamic_pointer_cast<_Dragon>(MyGraphics::ResourceManager::getGameObjectByName("Dragon"));
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_OpacityTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("OpacityDepthTexture"));
	m_pShader->setTextureUniformValue("u_CoefficientOneTex", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("CoefficientOne"));
	m_pShader->setTextureUniformValue("u_CoefficientTwoTex", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("CoefficientTwo"));
	auto NearPlane = MyGraphics::Camera::getMainCameraNear();
	auto FarPlane = MyGraphics::Camera::getMainCameraFar();
	m_pShader->setFloatUniformValue("u_NearPlane", static_cast<float>(NearPlane));
	m_pShader->setFloatUniformValue("u_FarPlane", static_cast<float>(FarPlane));
	m_pDragon->initModel(*m_pShader);
	MyGraphics::ResourceManager::registerSharedData("TansparentTexture", TextureTansparent);
}


void CRecontructPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	m_pShader->activeShader();
	glm::mat4 model = m_pDragon->getModelMatrix();
	for (int i = m_Location.size() - 1; i >= 0; i--)
	{
		glm::mat4 temp = glm::rotate(glm::translate(model, m_Location[i]), 1.57f, glm::vec3(0, 1, 0));
		m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(temp));
		m_pShader->setFloatUniformValue("u_DiffuseColor", m_Color[i].x, m_Color[i].y, m_Color[i].z, m_Color[i].w);
		m_pDragon->updateModel(*m_pShader);
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}