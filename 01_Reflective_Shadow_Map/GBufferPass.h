#pragma once
#include "RenderPass.h"

class _HalfCornellBox;
class Lucy;


class _GBufferPass : public _RenderPass
{
public:
	_GBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_GBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_HalfCornellBox> m_pHalfCornellBox;
	std::shared_ptr<Lucy> m_Lucy;
	int m_FBO;
};