#pragma once
#include "RenderPass.h"

class _Sponza;

class _ShadowmapPass : public _RenderPass
{
public:
	_ShadowmapPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_ShadowmapPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<_Sponza> m_pSponza;
	int m_FBO = -1;
	int m_ShadowmapResolution = 1024;	//1024
};