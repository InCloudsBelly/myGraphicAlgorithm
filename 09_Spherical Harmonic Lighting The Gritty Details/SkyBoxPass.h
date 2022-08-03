#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class _SkyboxPass : public _RenderPass
{
public:
	_SkyboxPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_SkyboxPass();

	virtual void initV() override;
	virtual void updateV() override;
private:
	GLint m_FBO;
};