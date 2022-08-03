#include "ShadingWithRSMPass.h"
#include "Common.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>

_ShadingWithRSMPass::_ShadingWithRSMPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_ShadingWithRSMPass::~_ShadingWithRSMPass()
{
}

void _ShadingWithRSMPass::initV()
{
	auto TextureConfig = std::make_shared<MyGraphics::STexture>(); ;
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_LINEAR;
	TextureConfig->Type4MagFilter = GL_LINEAR;
	TextureConfig->ImageBindUnit = 0;
	genTexture(TextureConfig);
	MyGraphics::ResourceManager::registerSharedData("ShadingTexture", TextureConfig);

	__initVPLsSampleCoordsAndWeights();

	m_LightVPMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightVPMatrix");

	m_pShader = std::make_shared<_Shader>("ShadingWithRSM_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_AlbedoTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_RSMFluxTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMFluxTexture"));
	m_pShader->setTextureUniformValue("u_RSMNormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMNormalTexture"));
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMPositionTexture"));
	m_pShader->setTextureUniformValue("u_RSMDepthTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMDepthTexture"));
	m_pShader->setFloatUniformValue("u_MaxSampleRadius", m_MaxSampleRadius);
	m_pShader->setIntUniformValue("u_RSMSize", MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution"));
	m_pShader->setIntUniformValue("u_VPLNum", m_VPLNum);
	m_LightDir = glm::vec4(MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir"), 0.0f);	//这是个方向，第四维需要是0，不能为1，否则会出问题

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
	glm::mat4 LightVPMatrix = m_LightVPMatrix ;
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightVPMatrix));
	glm::vec3 LightDir = normalize( m_LightDir);
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void _ShadingWithRSMPass::__initVPLsSampleCoordsAndWeights()
{
	std::default_random_engine e;
	std::uniform_real_distribution<float> u(0, 1);
	for (int i = 0; i < m_VPLNum; ++i)
	{
		float xi1 = u(e);
		float xi2 = u(e);
		m_VPLsSampleCoordsAndWeights.push_back({ xi1 * sin(2 * MyGraphics::PI * xi2), xi1 * cos(2 * MyGraphics::PI * xi2), xi1 * xi1, 0 });
	}

	genBuffer(GL_UNIFORM_BUFFER, m_VPLsSampleCoordsAndWeights.size() * 4 * sizeof(GL_FLOAT), m_VPLsSampleCoordsAndWeights.data(), GL_STATIC_DRAW, 1);
}