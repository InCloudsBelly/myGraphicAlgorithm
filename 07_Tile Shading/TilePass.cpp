#include "TilePass.h"
#include "Interface.h"
#include "LightSource.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>

_TilePass::_TilePass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_TilePass::~_TilePass()
{
}

void _TilePass::initV()
{
	auto pLightSources = MyGraphics::ResourceManager::getSharedDataByName<SPointLight*>("LightSources");
	auto LightSourecesByteSize = MyGraphics::ResourceManager::getSharedDataByName<size_t>("LightLightSourcesByteSize");
	auto LightNum = MyGraphics::ResourceManager::getSharedDataByName<size_t>("LightNum");
	genBuffer(GL_SHADER_STORAGE_BUFFER, LightSourecesByteSize, pLightSources, GL_STATIC_DRAW, 0);

	auto TextureConfig = std::make_shared<MyGraphics::STexture>();;
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_NEAREST;
	TextureConfig->Type4MagFilter = GL_NEAREST;
	TextureConfig->ImageBindUnit = 0;
	genTexture(TextureConfig);
	MyGraphics::ResourceManager::registerSharedData("FinalTexture", TextureConfig);

	m_pShader = std::make_shared<_Shader>("Tile_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_TotalLightNum", static_cast<int>(LightNum));
	m_pShader->setIntUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	std::shared_ptr<MyGraphics::STexture> AlbedoTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture");
	std::shared_ptr<MyGraphics::STexture> NormalTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture");
	std::shared_ptr<MyGraphics::STexture> PositionTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture");
	std::shared_ptr<MyGraphics::STexture> DepthTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("DepthTexture");
	m_pShader->setTextureUniformValue("u_AlbedoTexture", AlbedoTexture);
	m_pShader->setTextureUniformValue("u_NormalTexture", NormalTexture);
	m_pShader->setTextureUniformValue("u_PositionTexture", PositionTexture);
	m_pShader->setTextureUniformValue("u_DepthTexture", DepthTexture);
	const glm::mat4& ProjectionMatrix = MyGraphics::Camera::getMainCameraProjectionMatrix();
	m_pShader->setMat4UniformValue("u_InverseProjectionMatrix", glm::value_ptr(glm::inverse(ProjectionMatrix)));
	auto NearPlane = MyGraphics::Camera::getMainCameraNear();
	auto FarPlane = MyGraphics::Camera::getMainCameraFar();
	m_pShader->setFloatUniformValue("u_NearPlane", static_cast<float>(NearPlane));
	m_pShader->setFloatUniformValue("u_FarPlane", static_cast<float>(FarPlane));

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void _TilePass::updateV()
{
	m_pShader->activeShader();
	//glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	//glDispatchCompute(80, 45, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}