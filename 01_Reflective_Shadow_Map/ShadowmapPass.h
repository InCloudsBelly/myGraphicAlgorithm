#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class _HalfCornellBox;
class Lucy;

class _ShadowmapPass : public _RenderPass
{
public:
	_ShadowmapPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_ShadowmapPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<_HalfCornellBox> m_pHalfCornellBox;
	std::shared_ptr<Lucy> m_Lucy;
	int m_FBO = -1;
	int m_ShadowmapResolution = 1024;	//1024
};