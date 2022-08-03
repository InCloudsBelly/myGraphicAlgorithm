#pragma once
#include "RenderPass.h"

class _DrawTextureArrayByFBO : public _RenderPass
{
public:
	_DrawTextureArrayByFBO(const std::string& vPassName, int vExecutionOrder);
	virtual ~_DrawTextureArrayByFBO();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_InputTextureArray = -1;
};