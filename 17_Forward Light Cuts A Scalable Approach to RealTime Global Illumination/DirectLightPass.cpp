#include "DirectLightPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>

_DirectLightPass::_DirectLightPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_DirectLightPass::~_DirectLightPass()
{
}

void _DirectLightPass::initV()
{
	auto AlbedoTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture");
	auto NormalTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture");
	auto PositionTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture");
	auto LightDepthTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LightDepthTexture");

	auto TextureDirectIllumination = std::make_shared<MyGraphics::STexture>();
	TextureDirectIllumination->InternalFormat = GL_RGBA32F;
	TextureDirectIllumination->ExternalFormat = GL_RGBA;
	TextureDirectIllumination->DataType = GL_FLOAT;
	TextureDirectIllumination->ImageBindUnit = 0;
	genTexture(TextureDirectIllumination);

	m_pShader = std::make_shared<_Shader>("DirectLightPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputAlbedoTexture", AlbedoTexture);
	m_pShader->setTextureUniformValue("u_InputNormalTexture", NormalTexture);
	m_pShader->setTextureUniformValue("u_InputPositionTexture", PositionTexture);
	m_pShader->setTextureUniformValue("u_LightDepthTexture", LightDepthTexture);
	m_pShader->setImageUniformValue(TextureDirectIllumination);
	m_pShader->setFloatUniformValue("u_Intensity", MyGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", m_LightDir.x, m_LightDir.y, m_LightDir.z);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	MyGraphics::ResourceManager::registerSharedData("DirectIlluminationImage", TextureDirectIllumination);
}

void _DirectLightPass::updateV()
{
	m_pShader->activeShader();
	auto ViewMatrix = MyGraphics::Camera::getMainCameraViewMatrix();
	m_LightVPMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix") * MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");;
	m_pShader->setMat4UniformValue("u_LightVPMatrixMulInverseCameraViewMatrix", glm::value_ptr(m_LightVPMatrix));
	auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}