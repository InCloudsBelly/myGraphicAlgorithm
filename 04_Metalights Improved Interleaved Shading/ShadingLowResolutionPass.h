#pragma once
#include "RenderPass.h"
#include <vector>
#include "LightSource.h"
using namespace  std;


class _ShadingLowResolutionPass : public _RenderPass
{
public:
	_ShadingLowResolutionPass(const std::string& vPassName, int vExcutionOrder, MyGraphics::ERenderPassType vtype);
	virtual ~_ShadingLowResolutionPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	int ssbo;
	size_t LightSourecesByteSize;
	vector<_PointLight>* pLightSources;
	int LightNum;
	int blockx;
	int blocky;
};