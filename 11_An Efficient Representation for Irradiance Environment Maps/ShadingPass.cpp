#include "ShadingPass.h"
#include "Shader.h"
#include "Sponza.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


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

	auto m_LUTTexture = std::make_shared<MyGraphics::STexture>();
	loadTextureFromFile("../Textures/BRDFLUT/BRDFLut.dds", m_LUTTexture);
	getCoefs();
	MyGraphics::Camera::setMainCameraFarPlane(100);
	MyGraphics::Camera::setMainCameraPos({ -1.57278, 0.244948, 0.367194 });
	MyGraphics::Camera::setMainCameraFront({ 0.967832, -0.112856, -0.224865 });
	MyGraphics::Camera::setMainCameraMoveSpeed(0.1);
	m_pShader = std::make_shared<_Shader>("Sponza_VS.glsl", "Sponza_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_BRDFLut", m_LUTTexture);
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	for (int i = 0; i < m_Coefs.size(); i++)
	{
		m_pShader->setFloatUniformValue("u_Coef[" + std::to_string(i) + "]", m_Coefs[i].x, m_Coefs[i].y, m_Coefs[i].z);
	}
	m_pSponza->initModel(*m_pShader);
}

//************************************************************************************
//Function:
void _ShadingPass::getCoefs()
{
	std::string path = "../Textures/Skybox2/coefficients.txt";
	std::ifstream ifs(path);
	if (!ifs)
		throw std::runtime_error("open " + path + " failed");
	int i = 0;
	float r, g, b;
	while (ifs >> r >> g >> b)
	{
		m_Coefs.push_back(glm::vec3(r, g, b));
		i++;
	}
}

//************************************************************************************
//Function:
void _ShadingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_pShader->activeShader();
	m_pSponza->updateModel(*m_pShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}