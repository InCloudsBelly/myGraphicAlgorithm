#include "LightInjectPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
CLightInjectPass::CLightInjectPass(const std::string& vPassName, int vExecutionOrder) : _RenderPass(vPassName, vExecutionOrder)
{
}

CLightInjectPass::~CLightInjectPass()
{
}

void CLightInjectPass::drawVPLVAO()
{
	int RSMResolution = MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	if (m_VPLVAO == -1)
	{
		float *VPLVertices = new float[RSMResolution * RSMResolution];
		m_VPLVAO = createVAO(VPLVertices, RSMResolution * RSMResolution, { 2 });
	}
	glBindVertexArray(m_VPLVAO);
	glDrawArrays(GL_POINTS, 0, RSMResolution * RSMResolution);
	glBindVertexArray(0);
}


//************************************************************************************
//Function:
void CLightInjectPass::initV()
{
	int RSMResolution = MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	glm::vec3 MinAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	glm::vec3 MaxAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");

	m_Dimensions = MaxAABB - MinAABB;
	TextureLPVGridR = std::make_shared<MyGraphics::STexture>();
	TextureLPVGridG = std::make_shared<MyGraphics::STexture>();
	TextureLPVGridB = std::make_shared<MyGraphics::STexture>();
	TextureLPVGridR->ImageBindUnit = 0;
	TextureLPVGridG->ImageBindUnit = 1;
	TextureLPVGridB->ImageBindUnit = 2;
	TextureLPVGridR->TextureType = TextureLPVGridG->TextureType = TextureLPVGridB->TextureType = MyGraphics::STexture::ETextureType::Texture3D;
	TextureLPVGridR->InternalFormat = TextureLPVGridG->InternalFormat = TextureLPVGridB->InternalFormat = GL_R16F;
	TextureLPVGridR->ExternalFormat = TextureLPVGridG->ExternalFormat = TextureLPVGridB->ExternalFormat = GL_RGBA;
	TextureLPVGridR->DataType = TextureLPVGridG->DataType = TextureLPVGridB->DataType  = GL_FLOAT;
	TextureLPVGridR->Width = TextureLPVGridG->Width = TextureLPVGridB->Width = m_Dimensions.x;
	TextureLPVGridR->Height = TextureLPVGridG->Height = TextureLPVGridB->Height = m_Dimensions.y;
	TextureLPVGridR->Depth = TextureLPVGridG->Depth = TextureLPVGridB->Depth = m_Dimensions.z;
	TextureLPVGridR->Type4WrapS = TextureLPVGridG->Type4WrapS = TextureLPVGridB->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureLPVGridR->Type4WrapT = TextureLPVGridG->Type4WrapT = TextureLPVGridB->Type4WrapT = GL_CLAMP_TO_EDGE;

	genTexture(TextureLPVGridR);
	genTexture(TextureLPVGridG);
	genTexture(TextureLPVGridB);

	m_pShader = std::make_shared<_Shader>("LightInjectPass_VS.glsl", "LightInjectPass_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_MinAABB", MinAABB.x, MinAABB.y , MinAABB.z);
	m_pShader->setIntUniformValue("u_RSMResolution", RSMResolution);
	m_pShader->setTextureUniformValue("u_RSMRadiantFluxTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMRadiantFluxTexture"));
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMPositionTexture"));
	m_pShader->setTextureUniformValue("u_RSMNormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMNormalAndDoubleRoughnessTexture"));
	
	m_pShader->setImageUniformValue(TextureLPVGridR);
	m_pShader->setImageUniformValue(TextureLPVGridG);
	m_pShader->setImageUniformValue(TextureLPVGridB);

	MyGraphics::ResourceManager::registerSharedData("LPVGridRTexture", TextureLPVGridR);
	MyGraphics::ResourceManager::registerSharedData("LPVGridGTexture", TextureLPVGridG);
	MyGraphics::ResourceManager::registerSharedData("LPVGridBTexture", TextureLPVGridB);
}

//************************************************************************************
//Function:
void CLightInjectPass::updateV()
{
	ClearTexture(TextureLPVGridR, GL_TEXTURE_3D);
	ClearTexture(TextureLPVGridG, GL_TEXTURE_3D);
	ClearTexture(TextureLPVGridB, GL_TEXTURE_3D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Dimensions.x, m_Dimensions.y);
	m_pShader->activeShader();
	drawVPLVAO();
	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}