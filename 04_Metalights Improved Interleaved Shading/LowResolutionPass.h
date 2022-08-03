#pragma once
#include "RenderPass.h"
#include <vector>

class _LowResolutionPass : public _RenderPass
{
public:
	_LowResolutionPass(const std::string& vPassName, int vExcutionOrder, MyGraphics::ERenderPassType vtype);
	virtual ~_LowResolutionPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	int m_LowWidth;
	int m_LowHeight;
};