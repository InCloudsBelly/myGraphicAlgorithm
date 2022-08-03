#pragma once
#include "RenderPass.h"
#include <GL/glew.h>
class _Sponza;

class CAABBDebugPass : public _RenderPass
{
public:
	CAABBDebugPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CAABBDebugPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<_Sponza> m_pSponza;
	glm::vec3 m_Min;
	glm::vec3 m_Max;
};