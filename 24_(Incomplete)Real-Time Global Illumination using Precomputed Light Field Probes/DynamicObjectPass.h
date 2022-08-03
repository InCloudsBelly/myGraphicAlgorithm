#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class _DynamicObject;
class CDynamicObjectPass : public _RenderPass
{
public:
	CDynamicObjectPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CDynamicObjectPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_OldKeyMStatusSave = -1;
	int m_FBO;
	std::shared_ptr<_DynamicObject> m_pSponza;
};