#include "IndirectLightPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CIndirectLightPass::CIndirectLightPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

CIndirectLightPass::~CIndirectLightPass()
{
}

void CIndirectLightPass::initV()
{
	
	m_pShader = std::make_shared<_Shader>("IndirectLightPass_VS.glsl", "IndirectLightPass_FS.glsl");
	auto TextureIndirectLight = std::make_shared<MyGraphics::STexture>();
	TextureIndirectLight->InternalFormat = GL_RGBA32F;
	TextureIndirectLight->ExternalFormat = GL_RGBA;
	TextureIndirectLight->DataType = GL_FLOAT;
	TextureIndirectLight->isMipmap = false;
	genTexture(TextureIndirectLight);
	m_FBO = genFBO({ TextureIndirectLight });

	MyGraphics::ResourceManager::registerSharedData("IndirectLightTexture", TextureIndirectLight);
	glm::vec3 MinAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_MinAABB", MinAABB.x, MinAABB.y, MinAABB.z);
	m_pShader->setTextureUniformValue("u_RAccumulatorLPV", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVAccumulatorRTexture"));
	m_pShader->setTextureUniformValue("u_GAccumulatorLPV", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVAccumulatorGTexture"));
	m_pShader->setTextureUniformValue("u_BAccumulatorLPV", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVAccumulatorBTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture"));

}

void CIndirectLightPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();
	auto ViewMatrix = MyGraphics::Camera::getMainCameraViewMatrix();
	m_pShader->setMat4UniformValue("u_InverseCameraViewMatrix", glm::value_ptr(glm::inverse(ViewMatrix)));
	auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");


	drawQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}