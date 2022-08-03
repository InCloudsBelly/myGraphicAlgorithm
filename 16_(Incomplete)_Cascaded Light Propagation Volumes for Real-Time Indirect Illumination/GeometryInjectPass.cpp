#include "GeometryInjectPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
CGeometryInjectPass::CGeometryInjectPass(const std::string& vPassName, int vExecutionOrder) : _RenderPass(vPassName, vExecutionOrder)
{
}

CGeometryInjectPass::~CGeometryInjectPass()
{
}

void CGeometryInjectPass::drawVPLVAO()
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
void CGeometryInjectPass::initV()
{
	int RSMResolution = MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	glm::vec3 MinAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	glm::vec3 MaxAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");

	m_Dimensions = MaxAABB - MinAABB;
	TextureLPVGV = std::make_shared<MyGraphics::STexture>();
	TextureLPVGV->ImageBindUnit = 0;
	TextureLPVGV->TextureType = MyGraphics::STexture::ETextureType::Texture3D;
	TextureLPVGV->InternalFormat = GL_RGBA16F;
	TextureLPVGV->ExternalFormat = GL_RGBA;
	TextureLPVGV->DataType = GL_FLOAT;
	TextureLPVGV->Width = m_Dimensions.x;
	TextureLPVGV->Height = m_Dimensions.y;
	TextureLPVGV->Depth = m_Dimensions.z;
	TextureLPVGV->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureLPVGV->Type4WrapT = GL_CLAMP_TO_EDGE;
	genTexture(TextureLPVGV);


	m_pShader = std::make_shared<_Shader>("GeometryInjectPass_VS.glsl", "GeometryInjectPass_FS.glsl");
	m_pShader->activeShader();
	float test = MyGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace");
	glm::vec3 lightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_LightDir", lightDir.x, lightDir.y, lightDir.z);
	m_pShader->setFloatUniformValue("u_RSMArea", MyGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace"));
	m_pShader->setMat4UniformValue("u_LightViewMat", glm::value_ptr(MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix")));
	m_pShader->setFloatUniformValue("u_MinAABB", MinAABB.x, MinAABB.y , MinAABB.z);
	m_pShader->setIntUniformValue("u_RSMResolution", RSMResolution);
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMPositionTexture"));
	m_pShader->setTextureUniformValue("u_RSMNormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("RSMNormalAndDoubleRoughnessTexture"));
	m_pShader->setImageUniformValue(TextureLPVGV);

	MyGraphics::ResourceManager::registerSharedData("LPVGVTexture", TextureLPVGV);

}

//************************************************************************************
//Function:
void CGeometryInjectPass::updateV()
{
	ClearTexture(TextureLPVGV, GL_TEXTURE_3D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Dimensions.x, m_Dimensions.y);
	m_pShader->activeShader();
	drawVPLVAO();
	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}