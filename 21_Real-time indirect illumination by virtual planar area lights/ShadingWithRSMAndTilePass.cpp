#include "ShadingWithRSMAndTilePass.h"
#include "Common.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

CShadingWithRSMAndTilePass::CShadingWithRSMAndTilePass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

CShadingWithRSMAndTilePass::~CShadingWithRSMAndTilePass()
{
}

void CShadingWithRSMAndTilePass::initV()
{
	auto TextureConfig = std::make_shared<MyGraphics::STexture>(); ;
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_LINEAR;
	TextureConfig->Type4MagFilter = GL_LINEAR;
	TextureConfig->ImageBindUnit = 0;
	TextureConfig->isMipmap = false;
	genTexture(TextureConfig);
	MyGraphics::ResourceManager::registerSharedData("ShadingTexture", TextureConfig);

	int block = MyGraphics::ResourceManager::getSharedDataByName<int>("Block");
	m_pShader = std::make_shared<_Shader>("ShadingWithRSMAndTile_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_VALCount", block * block);
	m_pShader->setTextureUniformValue("u_LightDepthTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LightDepthTexture"));
	m_pShader->setFloatUniformValue("u_Intensity", MyGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_pShader->setTextureUniformValue("u_FilteredLightTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("BluredTexture"));
	m_pShader->setTextureUniformValue("u_NormalAndDoubleRoughnessTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_AlbedoTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));
	const glm::vec3& CameraPos = MyGraphics::Camera::getMainCameraPos();
	m_pShader->setFloatUniformValue("u_CameraPosInWorldSpace", CameraPos.x, CameraPos.y, CameraPos.z);
	m_LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");	
	m_pShader->setFloatUniformValue("u_LightDir", m_LightDir.x, m_LightDir.y, m_LightDir.z);
	const glm::mat4& ViewMatrix = MyGraphics::Camera::getMainCameraViewMatrix();
	m_pShader->setMat4UniformValue("u_InverseViewMatrix", glm::value_ptr(glm::inverse(ViewMatrix)));
	auto LightColor = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightColor");	//这是个方向，第四维需要是0，不能为1，否则会出问题
	m_pShader->setFloatUniformValue("u_LightColor", LightColor.x, LightColor.y, LightColor.z);
	m_pShader->setImageUniformValue(TextureConfig);
	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void CShadingWithRSMAndTilePass::updateV()
{
	m_pShader->activeShader();
	auto ViewMatrix = MyGraphics::Camera::getMainCameraViewMatrix();
	auto m_LightVPMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix") * MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	glm::mat4 LightViewMatrixMulInverseCameraViewMatrix = m_LightVPMatrix * glm::inverse(ViewMatrix);
	m_pShader->setMat4UniformValue("u_LightVPMatrixMulInverseCameraViewMatrix", glm::value_ptr(LightViewMatrixMulInverseCameraViewMatrix));
	const glm::vec3& CameraPos = MyGraphics::Camera::getMainCameraPos();
	m_pShader->setFloatUniformValue("u_CameraPosInWorldSpace", CameraPos.x, CameraPos.y, CameraPos.z);
	m_LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");	//这是个方向，第四维需要是0，不能为1，否则会出问题
	m_pShader->setFloatUniformValue("u_LightDir", m_LightDir.x, m_LightDir.y, m_LightDir.z);
	m_pShader->setMat4UniformValue("u_InverseViewMatrix", glm::value_ptr(glm::inverse(ViewMatrix)));
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}