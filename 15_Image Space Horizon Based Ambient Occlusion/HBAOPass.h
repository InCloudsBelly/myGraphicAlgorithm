#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class _HBAOPass : public _RenderPass
{
public:
	_HBAOPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_HBAOPass();

	virtual void initV();
	virtual void updateV();
	GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
	float *generateNoise();

private:
	int m_FBO;
};