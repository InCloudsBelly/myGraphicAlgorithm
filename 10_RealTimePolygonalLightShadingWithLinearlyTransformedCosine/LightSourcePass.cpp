#include "LightSourcePass.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Interface.h"
#include "GameObject.h"
#include "Utils.h"

_LightSourcePass::_LightSourcePass(const std::string& vPassName, int vExcutionOrder) :_RenderPass(vPassName, vExcutionOrder)
{
}

_LightSourcePass::~_LightSourcePass()
{
}

//************************************************************************************
//Function:
void _LightSourcePass::initV()
{
	m_pShader = std::make_shared<_Shader>("LightSource_VS.glsl", "LightSource_FS.glsl");
	std::shared_ptr<MyGraphics::STexture> m_LightSourceTexture = std::make_shared<MyGraphics::STexture>();
	loadTextureFromFile("Textures/0.png",m_LightSourceTexture);
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_LightSourceTexture", m_LightSourceTexture);
}

//************************************************************************************
//Function:
void _LightSourcePass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(MyGraphics::ResourceManager::getGameObjectByName("LightSource")->getModelMatrix()));

	float Intensity = MyGraphics::ResourceManager::getSharedDataByName<float>("Intensity");
	if (Intensity != m_Intensity)
	{
		m_Intensity = Intensity;
		m_pShader->setFloatUniformValue("u_Intensity", m_Intensity);
	}
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}