#include "ShadingLowResolutionPass.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"
#include "stack"
_ShadingLowResolutionPass::_ShadingLowResolutionPass(const std::string& vPassName, int vExcutionOrder, MyGraphics::ERenderPassType vtype) : _RenderPass(vPassName, vExcutionOrder, vtype)
{
}

_ShadingLowResolutionPass::~_ShadingLowResolutionPass()
{
}

void _ShadingLowResolutionPass::initV()
{
	 pLightSources = _LightSources::get_instance().getPointLights();
	//auto LightNum = MyGraphics::ResourceManager::getSharedDataByName<size_t>("LightNum");
	LightNum = pLightSources->size();
	LightSourecesByteSize = LightNum * sizeof(_PointLight);
	int LowWidth = MyGraphics::ResourceManager::getSharedDataByName<int>("LowWidth");
	int LowHeight = MyGraphics::ResourceManager::getSharedDataByName<int>("LowHeight");
	blockx = MyGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumX");
	blocky = MyGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumY");
	int SqrtLightNum = int(sqrt(LightNum));
	int groupX = SqrtLightNum;
	int groupY = SqrtLightNum;
	for (int i = SqrtLightNum; i >= 1; i--)
		if (LightNum % i == 0)
		{
			groupX = i;
			groupY = LightNum / i;
			break;
		}
	ssbo = genBuffer(GL_SHADER_STORAGE_BUFFER, LightSourecesByteSize, pLightSources->data(), GL_STATIC_DRAW, 0);
	m_pShader = std::make_shared<_Shader>("ShadingLowResolution_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_LowWidth", LowWidth);
	m_pShader->setIntUniformValue("u_LowHeight", LowHeight);
	m_pShader->setIntUniformValue("u_BlockX", groupX);
	m_pShader->setIntUniformValue("u_BlockY", groupY);
	m_pShader->setTextureUniformValue("u_AlbedoTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LowAlbedoImage"));
	m_pShader->setTextureUniformValue("u_NormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LowNormalImage"));
	m_pShader->setTextureUniformValue("u_PositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LowPositionImage"));

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((groupX + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((groupY + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}


bool cmp(const _PointLight& a, const _PointLight& b)
{
	return a.Importance.x > b.Importance.x; //从大到小排序
}
void _ShadingLowResolutionPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	_PointLight* p = (_PointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	vector<_PointLight> temp;
	vector<_PointLight> res;
	temp.resize(LightNum);
	for (int i = 0; i < LightNum; i++)
		temp[i] = p[i];
	sort(temp.begin(), temp.end(), cmp);
	int index = 0;
	int size = blockx * blocky;
	for (int i = 0; i < size; i++)
		for (int j = i; j < LightNum; j += size)
			res.push_back(temp[j]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, LightSourecesByteSize, res.data(), GL_DYNAMIC_DRAW);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}