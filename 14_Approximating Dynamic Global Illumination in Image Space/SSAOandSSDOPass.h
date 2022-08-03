#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class _SSAOandSSDOPass : public _RenderPass
{
public:
	_SSAOandSSDOPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_SSAOandSSDOPass();

	virtual void initV();
	virtual void updateV();
	GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
	std::vector<glm::vec3> generateKernel();
	std::vector<glm::vec3> generateNoise();

private:
	int m_FBO;
};