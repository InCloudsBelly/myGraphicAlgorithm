#include "ShadingPass.h"
#include "Shader.h"
#include "Sponza.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_ShadingPass::_ShadingPass(const std::string& vPassName, int vExcutionOrder) :_RenderPass(vPassName, vExcutionOrder)
{
}

_ShadingPass::~_ShadingPass()
{
}

//************************************************************************************
//Function:
void _ShadingPass::initV()
{
	MyGraphics::Camera::setMainCameraFarPlane(100);
	MyGraphics::Camera::setMainCameraPos({ -1.57278, 0.244948, 0.367194 });
	MyGraphics::Camera::setMainCameraFront({ 0.967832, -0.112856, -0.224865 });
	MyGraphics::Camera::setMainCameraMoveSpeed(0.5);
	m_pShader = std::make_shared<_Shader>("Sponza_VS.glsl", "Sponza_FS.glsl");

	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pSponza->initModel(*m_pShader);
}

//************************************************************************************
//Function:
void _ShadingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();
	
	CameraPos = MyGraphics::Camera::getMainCameraPos();
	m_pShader->setFloatUniformValue("CameraPos", CameraPos.x, CameraPos.y, CameraPos.z);

	m_pSponza->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}