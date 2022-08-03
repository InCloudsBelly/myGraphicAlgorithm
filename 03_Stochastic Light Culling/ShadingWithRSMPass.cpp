#include "ShadingWithRSMPass.h"
#include "Common.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

_ShadingWithRSMPass::_ShadingWithRSMPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_ShadingWithRSMPass::~_ShadingWithRSMPass()
{
}

void _ShadingWithRSMPass::initV()
{
	auto TextureConfig = std::make_shared<MyGraphics::STexture>();
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_LINEAR;
	TextureConfig->Type4MagFilter = GL_LINEAR;
	TextureConfig->ImageBindUnit = 0;
	genTexture(TextureConfig);
	MyGraphics::ResourceManager::registerSharedData("ShadingTexture", TextureConfig);

	m_LightViewMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_LightProjectionMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	m_LightVPMatrix = m_LightProjectionMatrix * m_LightViewMatrix;

	m_pShader = std::make_shared<_Shader>("ShadingWithRSM_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_AlbedoTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_LightDepthTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LightDepthTexture"));
	int RSMSize = MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	m_pShader->setIntUniformValue("u_RSMSize", RSMSize);
	m_pShader->setIntUniformValue("u_VPLsCount", RSMSize * RSMSize);
	m_pShader->setIntUniformValue("u_EnableIndirectIllumination", m_EnableIndirectIllumination);
	m_LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}


void _ShadingWithRSMPass::updateV()
{
	m_pShader->activeShader();
	auto ViewMatrix = MyGraphics::Camera::getMainCameraViewMatrix();
#ifdef _DEBUG
	m_LightVPMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix") * MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir"), 0.0f;
#endif
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(m_LightVPMatrix));
	m_pShader->setFloatUniformValue("u_LightDir", m_LightDir.x, m_LightDir.y, m_LightDir.z);
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_I) == GLFW_PRESS && m_OldKeyIStatus != GLFW_PRESS)
	{
		m_OldKeyIStatus = GLFW_PRESS;
		m_EnableIndirectIllumination = !m_EnableIndirectIllumination;
		m_pShader->setIntUniformValue("u_EnableIndirectIllumination", m_EnableIndirectIllumination);
	}
	else if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_I) == GLFW_RELEASE)
		m_OldKeyIStatus = GLFW_RELEASE;
}