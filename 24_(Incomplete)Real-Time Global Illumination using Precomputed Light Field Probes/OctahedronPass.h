#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class COctahedronPass : public _RenderPass
{
public:
	COctahedronPass(const std::string& vPassName, int vExecutionOrder, MyGraphics::ERenderPassType vtype);
	virtual ~COctahedronPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_FBO;
	int m_BakeResolution;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureRadiances;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureNormals;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureChebyshevs;

	std::shared_ptr<MyGraphics::STexture> m_TextureRadiance;
	std::shared_ptr<MyGraphics::STexture> m_TextureNormal;
	std::shared_ptr<MyGraphics::STexture> m_TextureChebyshev;
	std::shared_ptr<MyGraphics::STexture> m_TextureIrradiance;
	glm::ivec3 m_Min;
	glm::ivec3 m_Max;
};