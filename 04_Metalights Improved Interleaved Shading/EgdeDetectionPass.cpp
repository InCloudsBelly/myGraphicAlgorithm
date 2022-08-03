#include "EgdeDetectionPass.h"
#include "Shader.h"
#include "Utils.h"
#include "Interface.h"
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
_EdgeDetectionPass::_EdgeDetectionPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_EdgeDetectionPass::~_EdgeDetectionPass()
{
}

void _EdgeDetectionPass::initV()
{
	auto TextureConfig = std::make_shared<MyGraphics::STexture>();
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_NEAREST;
	TextureConfig->Type4MagFilter = GL_NEAREST;
	TextureConfig->ImageBindUnit = 0;
	genTexture(TextureConfig);
	MyGraphics::ResourceManager::registerSharedData("EdgeTexture", TextureConfig);

	m_pShader = std::make_shared<_Shader>("EgdeDetection_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	auto p = MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture");
	m_pShader->setTextureUniformValue("u_PositionTexture", p);
	m_pShader->setTextureUniformValue("u_NormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
	m_pShader->setFloatUniformValue("u_NormalThreshold", NormalThreshold);
	m_pShader->setFloatUniformValue("u_DepthThreshold", DepthThreshold);
	m_pShader->setImageUniformValue(TextureConfig);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((MyGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void _EdgeDetectionPass::updateV()
{
	m_pShader->activeShader();
	if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_O) == GLFW_PRESS && m_OldKeyADDNormalThreshold != GLFW_PRESS)
	{
		m_OldKeyADDNormalThreshold = GLFW_PRESS;
		NormalThreshold += 0.05f;
		m_pShader->setFloatUniformValue("u_NormalThreshold", NormalThreshold);
		std::cout << "u_NormalThreshold " << NormalThreshold << std::endl;
	}
	else if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_O) == GLFW_RELEASE)
		m_OldKeyADDNormalThreshold = GLFW_RELEASE;

	if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_PRESS && m_OldKeyDecreseNormalThreshold != GLFW_PRESS)
	{
		m_OldKeyDecreseNormalThreshold = GLFW_PRESS;
		NormalThreshold -= 0.05f;
		m_pShader->setFloatUniformValue("u_NormalThreshold", NormalThreshold);
		std::cout << "u_NormalThreshold " << NormalThreshold << std::endl;
	}
	else if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_RELEASE)
		m_OldKeyDecreseNormalThreshold = GLFW_RELEASE;

	if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_K) == GLFW_PRESS && m_OldKeyADDDepthThreshold != GLFW_PRESS)
	{
		m_OldKeyADDDepthThreshold = GLFW_PRESS;
		DepthThreshold += 0.05f;
		m_pShader->setFloatUniformValue("u_DepthThreshold", DepthThreshold);

		std::cout << "u_DepthThreshold " << DepthThreshold << std::endl;
	}
	else if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_K) == GLFW_RELEASE)
		m_OldKeyADDDepthThreshold = GLFW_RELEASE;

	if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_L) == GLFW_PRESS && m_OldKeyDecreseDepthThreshold != GLFW_PRESS)
	{
		m_OldKeyDecreseDepthThreshold = GLFW_PRESS;
		DepthThreshold -= 0.05f;
		m_pShader->setFloatUniformValue("u_DepthThreshold", DepthThreshold);

		std::cout << "u_DepthThreshold " << DepthThreshold << std::endl;
	}
	else if (MyGraphics::InputManager::getKeyStatus(GLFW_KEY_L) == GLFW_RELEASE)
		m_OldKeyDecreseDepthThreshold = GLFW_RELEASE;

	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}