#pragma once
#include "RenderPass.h"

class _Dragon;
class _Sponza;

struct ListNode
{
	unsigned packedColor;
	unsigned transmittance;
	unsigned depth;
	unsigned next;
};
class CreateLinkListPass : public _RenderPass
{
public:
	CreateLinkListPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CreateLinkListPass();

	virtual void initV();
	virtual void updateV();
	void clear();
private:
	int m_NodeSSBO;
	int m_AtomicSSBO;
	int m_HeadInitSSBO;
	GLuint *data;
	std::shared_ptr<MyGraphics::STexture> m_HeadTexture;

	std::shared_ptr<_Dragon> m_pDragon;
	std::shared_ptr<_Sponza> m_pSponza;
	std::vector<glm::vec3> m_Location = { glm::vec3(0, -1.7, 3.0),glm::vec3(0, -1.7, 2.0),glm::vec3(0, -1.7, 1.0),glm::vec3(0, -1.7, 0.0),glm::vec3(0, -1.7, -1.0)};
	std::vector<glm::vec4> m_Color = { glm::vec4(1.0,0,0,0.1),glm::vec4(1.0,1,0,0.1) ,glm::vec4(0.0,1,0,0.1) ,glm::vec4(0.0,0,1,0.1) ,glm::vec4(0.0,1,1,0.1) };
};