#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class _Sponza;

class CBakeProbePass : public _RenderPass
{
public:
	CBakeProbePass(const std::string& vPassName, int vExcutionOrder, MyGraphics::ERenderPassType vtype);
	virtual ~CBakeProbePass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<_Sponza> m_pSponza;

	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureAlbedos;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureNormals;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureDepths;
	std::vector<std::shared_ptr<MyGraphics::STexture>> m_TextureChebyshevs;
	glm::ivec3 m_MinAABB;
	glm::ivec3 m_MaxAABB;
	std::vector<int> m_FBOs;
	int m_BakeResolution = 128;
	glm::vec3 m_LightUpVector = glm::vec3(0, 1, 0);
	glm::vec3 m_BakeDir[6] = {
		glm::vec3(1,0,0),//+X
		glm::vec3(-1,0,0),//-X
		glm::vec3(0,1,0),//+Y
		glm::vec3(0,-1,0),//-Y
		glm::vec3(0,0,1),//+Z
		glm::vec3(0,0,-1)//+Z
	};

	float m_Fovy = glm::radians(45.0f);
	float m_Aspect = 1.0f;
	float m_Near = 0.1f; 
	float m_Far = 100.0f;
};