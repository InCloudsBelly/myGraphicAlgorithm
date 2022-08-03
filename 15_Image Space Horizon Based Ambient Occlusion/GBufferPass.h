#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class CornellBox;

class _GBufferPass : public _RenderPass
{
public:
	_GBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_GBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<CornellBox> m_pSponza;
	GLint m_FBO;
};