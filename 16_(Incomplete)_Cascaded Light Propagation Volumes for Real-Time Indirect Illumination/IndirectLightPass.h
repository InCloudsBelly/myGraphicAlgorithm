#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class CIndirectLightPass : public _RenderPass
{
public:
	CIndirectLightPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CIndirectLightPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_FBO;
};