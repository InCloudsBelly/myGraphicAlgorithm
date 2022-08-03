#pragma once
#include "RenderPass.h"
#include <vector>

class _SplitGBufferPass : public _RenderPass
{
public:
	_SplitGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_SplitGBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	int m_SubBufferNumX = 4;
	int m_SubBufferNumY = 4;
};