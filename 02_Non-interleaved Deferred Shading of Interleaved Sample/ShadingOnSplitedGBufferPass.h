#pragma once
#include "RenderPass.h"
#include <vector>

class _ShadingOnSplitedGBufferPass : public _RenderPass
{
public:
	_ShadingOnSplitedGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_ShadingOnSplitedGBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
};