#include "BakeProbePass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CBakeProbePass::CBakeProbePass(const std::string& vPassName, int vExcutionOrder, MyGraphics::ERenderPassType vtype) : _RenderPass(vPassName, vExcutionOrder, vtype)
{
}

CBakeProbePass::~CBakeProbePass()
{
}

void CBakeProbePass::initV()
{
	m_pShader = std::make_shared<_Shader>("BakeProbe_VS.glsl", "BakeProbe_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_MinAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	m_MaxAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	for (int i = m_MinAABB.x; i < m_MaxAABB.x; i++)
		for (int j = m_MinAABB.y; j < m_MaxAABB.y; j++)
			for (int k = m_MinAABB.z; k < m_MaxAABB.z; k++)
			{
				auto TextureAlbedo = std::make_shared<MyGraphics::STexture>();
				auto TextureNormal = std::make_shared<MyGraphics::STexture>();
				auto TextureChebyshev = std::make_shared<MyGraphics::STexture>();
				TextureAlbedo->InternalFormat = TextureNormal->InternalFormat = GL_RGBA32F;
				TextureAlbedo->ExternalFormat = TextureNormal->ExternalFormat = GL_RGBA;
				TextureAlbedo->DataType = TextureNormal->DataType = GL_FLOAT;
				TextureAlbedo->TextureType = TextureNormal->TextureType = MyGraphics::STexture::ETextureType::TextureCubeMap;
				TextureAlbedo->Width = TextureNormal->Width = m_BakeResolution;
				TextureAlbedo->Height = TextureNormal->Height = m_BakeResolution;

				TextureChebyshev->InternalFormat = GL_RGB32F;
				TextureChebyshev->ExternalFormat = GL_RGB;
				TextureChebyshev->DataType = GL_FLOAT;
				TextureChebyshev->TextureType = MyGraphics::STexture::ETextureType::TextureCubeMap;
				TextureChebyshev->Width = m_BakeResolution;
				TextureChebyshev->Height = m_BakeResolution;

				genTexture(TextureChebyshev);
				genTexture(TextureAlbedo);
				genTexture(TextureNormal);
				auto TextureDepth = std::make_shared<MyGraphics::STexture>();
				TextureDepth->InternalFormat = GL_RGBA32F;
				TextureDepth->ExternalFormat = GL_RGBA;
				TextureDepth->DataType = GL_FLOAT;
				TextureDepth->TextureType = MyGraphics::STexture::ETextureType::DepthCubeMap;
				TextureDepth->TextureAttachmentType = MyGraphics::STexture::_TextureAttachmentType::CubeDepthTexture;
				TextureDepth->Width = m_BakeResolution;
				TextureDepth->Height = m_BakeResolution;
				genTexture(TextureDepth);
				m_TextureAlbedos.push_back(TextureAlbedo);
				m_TextureNormals.push_back(TextureNormal);
				m_TextureDepths.push_back(TextureDepth);
				m_TextureChebyshevs.push_back(TextureChebyshev);
				auto FBO = genFBO({ TextureAlbedo, TextureNormal,TextureChebyshev, TextureDepth });
				m_FBOs.push_back(FBO);
			}

	MyGraphics::ResourceManager::registerSharedData("BakeResolution", m_BakeResolution);
	MyGraphics::ResourceManager::registerSharedData("BakeRadianceTextures", m_TextureAlbedos);
	MyGraphics::ResourceManager::registerSharedData("BakeNormalTextures", m_TextureNormals);
	MyGraphics::ResourceManager::registerSharedData("BakeChebyshevsTextures", m_TextureChebyshevs);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Near", MyGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", MyGraphics::Camera::getMainCameraFar());
	m_pSponza->initModel(*m_pShader);
}

void CBakeProbePass::updateV()
{
	int Index = 0;
	for (int i = m_MinAABB.x; i < m_MaxAABB.x; i++)
		for (int j = m_MinAABB.y; j < m_MaxAABB.y; j++)
			for (int k = m_MinAABB.z; k < m_MaxAABB.z; k++)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[Index]);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glViewport(0, 0, m_BakeResolution, m_BakeResolution);
				glm::vec3 ViewPos = glm::vec3(i, j, k);
				auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
				for (int i = 0; i < 6; i++)
				{
					glm::vec3 ViewDir = m_BakeDir[i];
					if (abs(ViewDir) == m_LightUpVector)
						ViewDir.z += 0.01f;
					glm::mat4x4 ViewMatrix = glm::lookAt(ViewPos, ViewPos + ViewDir, m_LightUpVector);
					glm::mat4x4 ProjectionMatrix = glm::perspective(m_Fovy, m_Aspect, m_Near, m_Far);

					auto DirLightVPMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix") * MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
					glm::mat4 LightViewMatrixMulInverseCameraViewMatrix = DirLightVPMatrix * glm::inverse(ViewMatrix);

					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureAlbedos[Index]->TextureID, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureNormals[Index]->TextureID, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureChebyshevs[Index]->TextureID, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureDepths[Index]->TextureID, 0);
					m_pShader->activeShader();
					m_pShader->setMat4UniformValue("u_BakeViewMatrix", glm::value_ptr(ViewMatrix));
					m_pShader->setMat4UniformValue("u_BakeProjectionMatrix", glm::value_ptr(ProjectionMatrix));
					m_pShader->setMat4UniformValue("u_LightVPMatrixMulInverseCameraViewMatrix", glm::value_ptr(LightViewMatrixMulInverseCameraViewMatrix));
					m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
					m_pShader->setFloatUniformValue("u_Intensity", MyGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
					m_pSponza->updateModel(*m_pShader);
				}
				Index++;
			}

	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}