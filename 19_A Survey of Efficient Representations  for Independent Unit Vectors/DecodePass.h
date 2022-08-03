#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class DecodePass : public _RenderPass
{
public:
	DecodePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~DecodePass();

	virtual void initV() override;
	virtual void updateV() override;
private:
	GLint m_FBO;
};