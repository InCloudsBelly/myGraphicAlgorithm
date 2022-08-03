#include "CalculateVPLRadiusPass.h"
#include "Utils.h"
#include "Interface.h"
#include "Shader.h"
#include <random>

_CalculateVPLRadiusPass::_CalculateVPLRadiusPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_CalculateVPLRadiusPass::~_CalculateVPLRadiusPass()
{
}

void _CalculateVPLRadiusPass::initV()
{
	int RSMSize = MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	int VPLsCount = RSMSize * RSMSize;
	size_t VPLsDataByeteSize = VPLsCount * sizeof(SVPL);
	genBuffer(GL_SHADER_STORAGE_BUFFER, VPLsDataByeteSize, nullptr, GL_STATIC_DRAW, 0);

	__gengrateRandomNumbers4VPLs();

	m_pShader = std::make_shared<_Shader>("CalculateVPLRadius_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_RSMSize", RSMSize);
	m_pShader->setIntUniformValue("u_VPLsCount", VPLsCount);
	m_pShader->setFloatUniformValue("u_MaxErrorBound", m_MaxErrorBound);
	m_pShader->setTextureUniformValue("u_RSMRandianceIntensityTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMRandianceIntensityTexture"));
	m_pShader->setTextureUniformValue("u_RSMNormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMNormalTexture"));
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMPositionTexture"));

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((RSMSize + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((RSMSize + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void _CalculateVPLRadiusPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void _CalculateVPLRadiusPass::__gengrateRandomNumbers4VPLs()
{
	std::default_random_engine e;
	std::uniform_real_distribution<float> u(0, 1);
	int RSMSize = MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	int VPLsCount = RSMSize * RSMSize;
	std::vector<float> RandomNumbers;
	RandomNumbers.resize(VPLsCount);
	for (int i = 0; i < VPLsCount; ++i)
		RandomNumbers[i] = u(e);
	genBuffer(GL_SHADER_STORAGE_BUFFER, RandomNumbers.size() * sizeof(float), RandomNumbers.data(), GL_STATIC_DRAW, 1);
}