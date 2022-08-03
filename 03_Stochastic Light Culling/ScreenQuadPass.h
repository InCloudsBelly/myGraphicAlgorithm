#pragma once
#include "RenderPass.h"

class _ScreenQuadPass : public _RenderPass
{
public:
	_ScreenQuadPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~_ScreenQuadPass();

	virtual void initV() override;
	virtual void updateV() override;
};