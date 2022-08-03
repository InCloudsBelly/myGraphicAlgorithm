#include "DynamicObjectPass.h"
#include "DynamicObject.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CDynamicObjectPass::CDynamicObjectPass(const std::string& vPassName, int vExecutionOrder) : _RenderPass(vPassName, vExecutionOrder)
{
}

CDynamicObjectPass::~CDynamicObjectPass()
{
}

//************************************************************************************
//Function:
void CDynamicObjectPass::initV()
{

	auto m_LUTTexture = std::make_shared<MyGraphics::STexture>();
	loadTextureFromFile("../Textures/BRDFLUT/ibl_brdf_lut.png", m_LUTTexture);

	m_pShader = std::make_shared<_Shader>("DynamicObject_VS.glsl", "DynamicObject_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_DynamicObject>(MyGraphics::ResourceManager::getGameObjectByName("DynamicObject"));

	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_Exposure", MyGraphics::ResourceManager::getSharedDataByName<float>("Exposure"));
	m_pShader->setTextureUniformValue("u_AlbedoTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_BRDFLut", m_LUTTexture);
	m_pShader->setTextureUniformValue("u_OutputOctRadianceImage", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("OctRadianceTextures"));
	m_pShader->setTextureUniformValue("u_OutputOctNormalImage", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("OctNormalTextures"));
	m_pShader->setTextureUniformValue("u_OutputOctChebyshevsImage", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("OctChebyshevsTextures"));
	m_pShader->setTextureUniformValue("u_OutputOctIrradianceImage", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("OctIrradianceTextures"));
	auto MinAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	auto MaxAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	glm::ivec3 CellNum = glm::ivec3((int)MaxAABB.x - (int)MinAABB.x, (int)MaxAABB.y - (int)MinAABB.y, (int)MaxAABB.z - (int)MinAABB.z);
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_MinAABB", MinAABB.x, MinAABB.y, MinAABB.z);
	m_pShader->setIntUniformValue("u_CellNum", CellNum.x, CellNum.y, CellNum.z);
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pSponza->initModel(*m_pShader);
}

//************************************************************************************
//Function:
void CDynamicObjectPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();
	m_pSponza->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}