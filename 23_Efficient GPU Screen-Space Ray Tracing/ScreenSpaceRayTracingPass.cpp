#include "ScreenSpaceRayTracingPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "AABB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_ScreenSpaceRayTracingPass::_ScreenSpaceRayTracingPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_ScreenSpaceRayTracingPass::~_ScreenSpaceRayTracingPass()
{
}

void _ScreenSpaceRayTracingPass::initV()
{
	m_pShader = std::make_shared<_Shader>("GBuffer_VS.glsl", "GBuffer_FS.glsl");
	m_pDynamicObjectShader = std::make_shared<_Shader>("ScreenSpaceRayTracing_VS.glsl", "ScreenSpaceRayTracing_FS.glsl");

	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pDynamicObject = std::dynamic_pointer_cast<_DynamicObject>(MyGraphics::ResourceManager::getGameObjectByName("DynamicObject"));


	auto TextureRayMarching = std::make_shared<MyGraphics::STexture>();
	TextureRayMarching->InternalFormat = GL_RGBA32F;
	TextureRayMarching->ExternalFormat = GL_RGBA;
	TextureRayMarching->DataType = GL_FLOAT;
	genTexture(TextureRayMarching);

	m_FBO = genFBO({ TextureRayMarching });

	MyGraphics::ResourceManager::registerSharedData("RayMarchingTexture", TextureRayMarching);


	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Near", MyGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", MyGraphics::Camera::getMainCameraFar());
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(MyGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->initModel(*m_pShader);


	m_pDynamicObjectShader->activeShader();
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pDynamicObjectShader->setFloatUniformValue("u_Near", MyGraphics::Camera::getMainCameraNear());
	m_pDynamicObjectShader->setFloatUniformValue("u_Far", MyGraphics::Camera::getMainCameraFar());
	m_pDynamicObjectShader->setFloatUniformValue("u_DepthSize", 1920.0f,1152.0f);
	m_pDynamicObjectShader->setTextureUniformValue("u_DepthTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("DepthTexture"));
	m_pDynamicObjectShader->setTextureUniformValue("u_AlbedoTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("AlbedoTexture"));
	m_pDynamicObjectShader->setFloatUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pDynamicObjectShader->setFloatUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	
}

void _ScreenSpaceRayTracingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(MyGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->updateModel(*m_pShader);

	m_pDynamicObjectShader->activeShader();
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pDynamicObject->getModelMatrix()));
	glm::vec3 CameraPos = MyGraphics::Camera::getMainCameraPos();
	m_pDynamicObjectShader->setFloatUniformValue("u_CameraPosInWorldSpace", CameraPos.x, CameraPos.y, CameraPos.z);
	drawQuad();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}