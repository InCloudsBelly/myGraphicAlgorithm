#include "CalculateWeightPass.h"
#include "Utils.h"
#include "Interface.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>
#include "vector"
#include <GLFW/glfw3.h>
using namespace std;
CCalculateWeight::CCalculateWeight(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

CCalculateWeight::~CCalculateWeight()
{
}

void CCalculateWeight::initV()
{
	int block = MyGraphics::ResourceManager::getSharedDataByName<int>("Block");
	int resolution = MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");

	m_TextureAlbedoAndMetallic = std::make_shared<MyGraphics::STexture>();
	m_TextureAlbedoAndMetallic->InternalFormat = GL_RGBA32F;
	m_TextureAlbedoAndMetallic->ExternalFormat = GL_RGBA;
	m_TextureAlbedoAndMetallic->DataType = GL_FLOAT;
	m_TextureAlbedoAndMetallic->Type4MinFilter = GL_LINEAR;
	m_TextureAlbedoAndMetallic->Type4MagFilter = GL_LINEAR;
	m_TextureAlbedoAndMetallic->ImageBindUnit = 0;
	m_TextureAlbedoAndMetallic->isMipmap = true;
	m_TextureAlbedoAndMetallic->Height = m_TextureAlbedoAndMetallic->Width = resolution;
	genTexture(m_TextureAlbedoAndMetallic);

	m_pShader = std::make_shared<_Shader>("CalculateWeight_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_VALCount", block * block);
	m_pShader->setIntUniformValue("u_Block", block);
	m_pShader->setIntUniformValue("u_RSMResolution", MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution"));
	m_pShader->setTextureUniformValue("u_RSMAlbedoAndMetallicTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMAlbedoAndMetallicTexture"));
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMPositionTexture"));
	m_pShader->setTextureUniformValue("u_RSMNormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMNormalAndDoubleRoughnessTexture"));
	m_pShader->setImageUniformValue(m_TextureAlbedoAndMetallic);
	
	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((resolution + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((resolution + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	MyGraphics::ResourceManager::registerSharedData("WeightAlbedoImage", m_TextureAlbedoAndMetallic);
}

void CCalculateWeight::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	genGenerateMipmap(m_TextureAlbedoAndMetallic);
	
}
