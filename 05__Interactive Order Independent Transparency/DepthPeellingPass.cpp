#include "DepthPeellingPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
DepthPeellingPass::DepthPeellingPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

DepthPeellingPass::~DepthPeellingPass()
{
}

void DepthPeellingPass::initV()
{
	MyGraphics::Camera::setMainCameraFarPlane(100);
	MyGraphics::Camera::setMainCameraMoveSpeed(0.1);

	m_pShader = std::make_shared<_Shader>("DepthPeelling_VS.glsl", "DepthPeelling_FS.glsl");
	
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	for (int i = 0; i < 2; i++)
	{
		auto TextureConfig = std::make_shared<MyGraphics::STexture>();
		TextureConfig->InternalFormat = GL_RGBA32F_ARB;
		TextureConfig->ExternalFormat = GL_RGBA;
		TextureConfig->DataType = GL_FLOAT;
		genTexture(TextureConfig);
		m_TextureColor.push_back(TextureConfig);

		auto TextureDepth = std::make_shared<MyGraphics::STexture>();
		TextureDepth->InternalFormat = GL_DEPTH_COMPONENT32F;
		TextureDepth->ExternalFormat = GL_DEPTH_COMPONENT;
		TextureDepth->DataType = GL_FLOAT;
		TextureDepth->Type4MinFilter = GL_NEAREST;
		TextureDepth->Type4MagFilter = GL_NEAREST;
		TextureDepth->TextureAttachmentType = MyGraphics::STexture::_TextureAttachmentType::DepthTexture;

		genTexture(TextureDepth);
		m_TextureDepth.push_back(TextureDepth);

		int FBO = genFBO({ m_TextureColor[i],m_TextureDepth[i]});
		m_FBO.push_back(FBO);
	}


	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_Width", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setFloatUniformValue("u_Height", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::vec3 m_LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");	
	m_pShader->setFloatUniformValue("u_LightDir", m_LightDir.x, m_LightDir.y, m_LightDir.z);
	glm::vec3 viewPos = MyGraphics::Camera::getMainCameraPos();	
	m_pShader->setFloatUniformValue("u_ViewPos", viewPos.x, viewPos.y, viewPos.z);
	m_pSponza->initModel(*m_pShader);

	//BlendPass
	

	m_TextureConfig = std::make_shared<MyGraphics::STexture>();
	m_TextureConfig->InternalFormat = GL_RGBA32F_ARB;
	m_TextureConfig->ExternalFormat = GL_RGBA;
	m_TextureConfig->DataType = GL_FLOAT;
	genTexture(m_TextureConfig);

	auto TextureDepth = std::make_shared<MyGraphics::STexture>();
	TextureDepth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureDepth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureDepth->DataType = GL_FLOAT;
	TextureDepth->Type4MinFilter = GL_NEAREST;
	TextureDepth->Type4MagFilter = GL_NEAREST;
	TextureDepth->TextureAttachmentType = MyGraphics::STexture::_TextureAttachmentType::DepthTexture;
	genTexture(TextureDepth);
	m_BlendFBO = genFBO({ m_TextureConfig, TextureDepth});

	m_BlendShader = std::make_shared<_Shader>("BlendPass_VS.glsl", "BlendPass_FS.glsl");
	m_BlendShader->activeShader();
	m_BlendShader->setTextureUniformValue("u_BlendTexture", m_TextureColor[0]);
	m_BlendShader->setTextureUniformValue("u_ColorTexture", m_TextureColor[0]);
	MyGraphics::ResourceManager::registerSharedData("ShadingTexture", m_TextureConfig);

}

void DepthPeellingPass::updateV()
{
	for (int i = 0; i < 4; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[i % 2]);		//按照 [0, 1, 0, 1]的顺序generate depth map
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		m_pShader->activeShader();
		m_pShader->setIntUniformValue("u_Layer", i);
		glm::vec3 viewPos = MyGraphics::Camera::getMainCameraPos();	
		m_pShader->setFloatUniformValue("u_ViewPos", viewPos.x, viewPos.y, viewPos.z);
		if (i != 0)
			m_pShader->setTextureUniformValue("u_DepthTexture", m_TextureDepth[(i - 1) % 2]);   //按照[#， 0， 1， 0]的顺序读取 depth map 用于cull surface
		m_pSponza->updateModel(*m_pShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_BlendFBO);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		m_BlendShader->activeShader();
		if (i != 0)
		{
			m_BlendShader->setTextureUniformValue("u_BlendTexture", m_TextureConfig);
			m_BlendShader->setTextureUniformValue("u_ColorTexture", m_TextureColor[(i - 1) % 2]);
		}
		else
		{
			m_BlendShader->setTextureUniformValue("u_BlendTexture", m_TextureColor[0]);
			m_BlendShader->setTextureUniformValue("u_ColorTexture", m_TextureColor[0]);
		}
		drawQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


}