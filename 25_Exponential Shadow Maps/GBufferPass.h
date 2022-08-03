#pragma once
#include "RenderPass.h"
#include <GL/glew.h>
#include "DynamicObject.h"
class _Sponza;

class GBufferPass : public _RenderPass
{
public:
	GBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~GBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Shader>m_pDynamicObjectShader;
	std::shared_ptr<_Sponza> m_pSponza;
	std::shared_ptr<_DynamicObject> m_pDynamicObject;
	int m_FBO;
};