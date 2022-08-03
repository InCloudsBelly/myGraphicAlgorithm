#include "CascadedShadowMapPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "LightCamera.h"
#include "AABB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


CascadedShadowMapPass::CascadedShadowMapPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

CascadedShadowMapPass::~CascadedShadowMapPass()
{
}

void CascadedShadowMapPass::initV()
{
	m_SplitNum = MyGraphics::ResourceManager::getSharedDataByName<int>("SplitNum");
	m_pShader = std::make_shared<_Shader>("CascadedShadowMap_VS.glsl", "CascadedShadowMap_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pCLightCamera = std::dynamic_pointer_cast<_LightCamera>(MyGraphics::ResourceManager::getGameObjectByName("LightCamera"));

	auto TextureDepth = std::make_shared<MyGraphics::STexture>(); ;
	TextureDepth->InternalFormat = GL_RGBA16F;
	TextureDepth->ExternalFormat = GL_RGBA;
	TextureDepth->DataType = GL_FLOAT;
	TextureDepth->Width = TextureDepth->Height = m_RSMResolution;
	TextureDepth->Depth = MyGraphics::ResourceManager::getSharedDataByName<int>("SplitNum");
	TextureDepth->TextureType = MyGraphics::STexture::ETextureType::Texture2DArray;
	TextureDepth->BorderColor = { 0,0,0,0 };
	TextureDepth->ImageBindUnit = 0;
	genTexture(TextureDepth);

	MyGraphics::ResourceManager::registerSharedData("LightDepthTexture", TextureDepth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Intensity", MyGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_pShader->setImageUniformValue(TextureDepth);
	m_pSponza->initModel(*m_pShader);
}

void CascadedShadowMapPass::updateV()
{
	std::vector<glm::mat4> Frustum = m_pCLightCamera->getFrustum();
	glm::mat4 LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	
	for (int i = 0; i < Frustum.size(); i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glViewport(0, 0, m_RSMResolution, m_RSMResolution);
		m_pShader->activeShader();
		m_pShader->setIntUniformValue("u_Index", i);
		m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(Frustum[i] * LightViewMatrix));
		m_pSponza->updateModel(*m_pShader);
		glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}