#include "OctahedronPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
COctahedronPass::COctahedronPass(const std::string& vPassName, int vExecutionOrder, MyGraphics::ERenderPassType vtype) : _RenderPass(vPassName, vExecutionOrder, vtype)
{
}

COctahedronPass::~COctahedronPass()
{
}

//************************************************************************************
//Function:
void COctahedronPass::initV()
{
	m_BakeResolution = MyGraphics::ResourceManager::getSharedDataByName<int>("BakeResolution");
	m_Min = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	m_Max = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	m_TextureRadiances = MyGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<MyGraphics::STexture>>>("BakeRadianceTextures");
	m_TextureNormals = MyGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<MyGraphics::STexture>>>("BakeNormalTextures");
	m_TextureChebyshevs = MyGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<MyGraphics::STexture>>>("BakeChebyshevsTextures");
	
	m_TextureRadiance = std::make_shared<MyGraphics::STexture>();
	m_TextureNormal = std::make_shared<MyGraphics::STexture>();
	m_TextureChebyshev = std::make_shared<MyGraphics::STexture>();
	m_TextureRadiance->InternalFormat = m_TextureNormal->InternalFormat = GL_RGBA16F;
	m_TextureRadiance->ExternalFormat = m_TextureNormal->ExternalFormat = GL_RGBA;
	m_TextureRadiance->DataType = m_TextureNormal->DataType = GL_FLOAT;
	m_TextureRadiance->TextureType = m_TextureNormal->TextureType = MyGraphics::STexture::ETextureType::Texture2DArray;
	m_TextureRadiance->Width = m_TextureNormal->Width = m_BakeResolution;
	m_TextureRadiance->Height = m_TextureNormal->Height = m_BakeResolution;
	m_TextureRadiance->Depth = m_TextureNormal->Depth = m_TextureRadiances.size();

	m_TextureChebyshev->InternalFormat = GL_RGBA16F;
	m_TextureChebyshev->ExternalFormat = GL_RGBA;
	m_TextureChebyshev->DataType = GL_FLOAT;
	m_TextureChebyshev->TextureType = MyGraphics::STexture::ETextureType::Texture2DArray;
	m_TextureChebyshev->Width = m_BakeResolution;
	m_TextureChebyshev->Height = m_BakeResolution;
	m_TextureChebyshev->Depth = m_TextureRadiances.size();
	m_TextureChebyshev->Type4WrapS = m_TextureNormal->Type4WrapS = m_TextureRadiance->Type4WrapS = GL_CLAMP_TO_EDGE;
	m_TextureChebyshev->Type4WrapT = m_TextureNormal->Type4WrapT = m_TextureRadiance->Type4WrapT = GL_CLAMP_TO_EDGE;
	m_TextureChebyshev->Type4WrapR = m_TextureNormal->Type4WrapR = m_TextureRadiance->Type4WrapR = GL_CLAMP_TO_EDGE;

	m_TextureIrradiance = std::make_shared<MyGraphics::STexture>();
	m_TextureIrradiance->InternalFormat = GL_RGBA16F;
	m_TextureIrradiance->ExternalFormat = GL_RGBA;
	m_TextureIrradiance->DataType = GL_FLOAT;
	m_TextureIrradiance->TextureType = MyGraphics::STexture::ETextureType::Texture2DArray;
	m_TextureIrradiance->Width = m_BakeResolution;
	m_TextureIrradiance->Height = m_BakeResolution;
	m_TextureIrradiance->Depth = m_TextureRadiances.size();


	m_TextureRadiance->ImageBindUnit = 0;
	m_TextureNormal->ImageBindUnit = 1;
	m_TextureChebyshev->ImageBindUnit = 2;
	m_TextureIrradiance->ImageBindUnit = 3;

	genTexture(m_TextureChebyshev);
	genTexture(m_TextureRadiance);
	genTexture(m_TextureNormal);
	genTexture(m_TextureIrradiance);



	m_pShader = std::make_shared<_Shader>("OctahedronPass_VS.glsl", "OctahedronPass_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setImageUniformValue(m_TextureRadiance);
	m_pShader->setImageUniformValue(m_TextureNormal);
	m_pShader->setImageUniformValue(m_TextureChebyshev);
	m_pShader->setImageUniformValue(m_TextureIrradiance);

	MyGraphics::ResourceManager::registerSharedData("OctRadianceTextures", m_TextureRadiance);
	MyGraphics::ResourceManager::registerSharedData("OctNormalTextures", m_TextureNormal);
	MyGraphics::ResourceManager::registerSharedData("OctChebyshevsTextures", m_TextureChebyshev);
	MyGraphics::ResourceManager::registerSharedData("OctIrradianceTextures", m_TextureIrradiance);
}
	
//************************************************************************************
//Function:
void COctahedronPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_BakeResolution, m_BakeResolution);
	int Index = 0;
	for (int i = m_Min.x; i < m_Max.x; i++)
		for (int j = m_Min.y; j < m_Max.y; j++)
			for (int k = m_Min.z; k < m_Max.z; k++)
			{
				m_pShader->activeShader();
				m_pShader->setTextureUniformValue("u_BakeRadianceTextures", m_TextureRadiances[Index]);
				m_pShader->setTextureUniformValue("u_BakeNormalTextures", m_TextureNormals[Index]);
				m_pShader->setTextureUniformValue("u_BakeChebyshevsTextures", m_TextureChebyshevs[Index]);
				m_pShader->setIntUniformValue("u_Index", Index);
				m_pShader->setIntUniformValue("u_BakeResolution", m_BakeResolution);
				drawQuad();
				Index++;
			}
	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}