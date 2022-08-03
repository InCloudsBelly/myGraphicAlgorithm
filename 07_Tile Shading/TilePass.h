#pragma once
#include "RenderPass.h"
#include <vector>

class _TilePass : public _RenderPass
{
public:
	_TilePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_TilePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
};