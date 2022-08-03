#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CLightInjectPass : public _RenderPass
{
public:
	CLightInjectPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CLightInjectPass();

	virtual void initV() override;
	virtual void updateV() override;
	void drawVPLVAO();
private:
	glm::ivec3 m_Dimensions;
	int m_FBO = -1;
	int m_VPLVAO = -1;
	std::shared_ptr<MyGraphics::STexture> TextureLPVGridR;
	std::shared_ptr<MyGraphics::STexture> TextureLPVGridG;
	std::shared_ptr<MyGraphics::STexture> TextureLPVGridB;
};