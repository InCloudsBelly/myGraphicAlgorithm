#pragma once
#include "RenderPass.h"
#include <vector>
#include "Common.h"
class CNormalComputePass : public _RenderPass
{
public:
	CNormalComputePass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CNormalComputePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_PingPongFBO = -1;
	std::shared_ptr<MyGraphics::STexture> m_FirstTexture;
	std::shared_ptr<MyGraphics::STexture> m_SecondTexture;
	std::shared_ptr<MyGraphics::STexture> m_PositionTexture;
	std::shared_ptr<MyGraphics::STexture> m_NormalTexture;
	int m_MaxMipLevel = 10;
	int RSMsize;
};