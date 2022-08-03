#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class _Sponza;

class _ShadingPass : public _RenderPass
{
public:
	_ShadingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_ShadingPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	glm::vec3 CameraPos;
	std::shared_ptr<_Sponza> m_pSponza;
	GLint m_FBO;
};