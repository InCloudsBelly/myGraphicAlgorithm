#pragma once
#include "RenderPass.h"
#include <vector>

class _GatherBufferPass : public _RenderPass
{
public:
	_GatherBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_GatherBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
};