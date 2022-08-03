#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include "Common.h"
class _LightSourcePass : public _RenderPass
{
public:
	_LightSourcePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_LightSourcePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	float m_Intensity = 0.0;
	int m_FBO = -1;
	int  m_OldKeyPStatus = -1;
	bool m_IsRender = true;
	std::shared_ptr<MyGraphics::STexture>   m_LightSourceTexture = 0;
	std::vector<unsigned int>vbos;
	std::vector<unsigned int>vaos;
};