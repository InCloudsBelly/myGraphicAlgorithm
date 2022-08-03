#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include "DynamicObject.h"
class _Sponza;

class CRSMBufferPass : public _RenderPass
{
public:
	CRSMBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CRSMBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Shader>m_pDynamicObjectShader;
	std::shared_ptr<_Sponza> m_pSponza;
	std::shared_ptr<_DynamicObject> m_pDynamicObject;
	int m_FBO = -1;
	int m_RSMResolution = 128;	
	GLuint m_Query = -1;
	GLuint64 m_ElapsedTime = -1;
};