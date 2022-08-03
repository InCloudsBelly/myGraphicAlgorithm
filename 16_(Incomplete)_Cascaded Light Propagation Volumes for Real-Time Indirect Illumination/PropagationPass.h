#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CPropagationPass : public _RenderPass
{
public:
	CPropagationPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CPropagationPass();

	virtual void initV() override;
	virtual void updateV() override;
	void drawVPLVAO();
private:
	glm::ivec3 m_Dimensions;
	int m_FBO = -1;
	int m_VPLVAO = -1;
	int m_ProgationNum = 8;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureLPVGridR;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureLPVGridG;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureLPVGridB;

	std::shared_ptr<MyGraphics::STexture> m_TextureLPVAccumulatorR;
	std::shared_ptr<MyGraphics::STexture> m_TextureLPVAccumulatorG;
	std::shared_ptr<MyGraphics::STexture> m_TextureLPVAccumulatorB;
};