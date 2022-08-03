#include "CSMDebugPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "LightCamera.h"
_CSMDebugPass::_CSMDebugPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_CSMDebugPass::~_CSMDebugPass()
{
}

void _CSMDebugPass::initV()
{
	auto PositionTexture = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture");
	m_pCLightCamera = std::dynamic_pointer_cast<_LightCamera>(MyGraphics::ResourceManager::getGameObjectByName("LightCamera"));

	auto TextureCSMDebug = std::make_shared<MyGraphics::STexture>();
	TextureCSMDebug->InternalFormat = GL_RGBA32F;
	TextureCSMDebug->ExternalFormat = GL_RGBA;
	TextureCSMDebug->DataType = GL_FLOAT;
	TextureCSMDebug->ImageBindUnit = 0;
	genTexture(TextureCSMDebug);

	m_pShader = std::make_shared<_Shader>("CSMDebugPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputPositionTexture", PositionTexture);
	m_pShader->setImageUniformValue(TextureCSMDebug);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	MyGraphics::ResourceManager::registerSharedData("CSMDebugImage", TextureCSMDebug);
}

void _CSMDebugPass::updateV()
{
	std::vector<glm::mat4> Frustum = m_pCLightCamera->getFrustum();

	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_SpiltNum", Frustum.size());
	auto ViewMatrix = MyGraphics::Camera::getMainCameraViewMatrix();
	for (int i = 0; i < Frustum.size(); i++)
	{
		m_LightVPMatrix = Frustum[i] * MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
		m_pShader->setMat4UniformValue("u_LightVPMatrixMulInverseCameraViewMatrix[" + std::to_string(i) +"]", glm::value_ptr(m_LightVPMatrix));

	}
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}