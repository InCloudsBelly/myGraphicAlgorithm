#include "HBAOPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include <GLM/gtc/random.hpp>
#include <random>

_HBAOPass::_HBAOPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_HBAOPass::~_HBAOPass()
{
}

void _HBAOPass::initV()
{
	m_pShader = std::make_shared<_Shader>("HBAOPass_VS.glsl", "HBAOPass_FS.glsl");
	float *HBAONoise = generateNoise();
	auto TextureConfigHBAO = std::make_shared<MyGraphics::STexture>();
	auto TextureConfigNoise = std::make_shared<MyGraphics::STexture>();
	TextureConfigHBAO->InternalFormat = GL_R16;
	TextureConfigHBAO->ExternalFormat = GL_RED;
	TextureConfigHBAO->DataType = GL_FLOAT;

	TextureConfigNoise->InternalFormat = GL_RGBA16F;
	TextureConfigNoise->Width = 4;
	TextureConfigNoise->Height = 4;
	TextureConfigNoise->Type4WrapS = GL_REPEAT;
	TextureConfigNoise->Type4WrapT = GL_REPEAT;
	TextureConfigNoise->Type4MinFilter = GL_NEAREST;
	TextureConfigNoise->Type4MagFilter = GL_NEAREST;

	TextureConfigNoise->ExternalFormat = GL_RGBA;
	TextureConfigNoise->DataType = GL_FLOAT;
	TextureConfigNoise->pDataSet.push_back(HBAONoise);

	genTexture(TextureConfigHBAO);
	genTexture(TextureConfigNoise);

	m_FBO = genFBO({ TextureConfigHBAO});

	MyGraphics::ResourceManager::registerSharedData("HBAOTexture", TextureConfigHBAO);
	MyGraphics::ResourceManager::registerSharedData("NoiseTexture", TextureConfigNoise);

	glm::vec2 FocalLen, InvFocalLen, UVToViewA, UVToViewB, LinMAD;
	float fovRad = glm::radians(MyGraphics::Camera::getMainCameraFov());
	FocalLen[0] = 1.0f / tanf(fovRad * 0.5f) * ((float)MyGraphics::WINDOW_KEYWORD::getWindowHeight() / (float)MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	FocalLen[1] = 1.0f / tanf(fovRad * 0.5f);
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setFloatUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setFloatUniformValue("u_Near", MyGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Fov", glm::radians(MyGraphics::Camera::getMainCameraFov()));
	m_pShader->setFloatUniformValue("u_Far", MyGraphics::Camera::getMainCameraFar());
	m_pShader->setFloatUniformValue("u_FocalLen", FocalLen.x, FocalLen.y);
	m_pShader->setTextureUniformValue("u_DepthTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("DepthTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_NoiseTexture", TextureConfigNoise);

}

void _HBAOPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float *_HBAOPass::generateNoise()
{
	float *noise = new float[4 * 4 * 4];
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			glm::vec2 xy = glm::circularRand(1.0f);
			float z = glm::linearRand(0.0f, 1.0f);
			float w = glm::linearRand(0.0f, 1.0f);

			int offset = 4 * (y * 4 + x);
			noise[offset + 0] = xy[0];
			noise[offset + 1] = xy[1];
			noise[offset + 2] = z;
			noise[offset + 3] = w;
		}
	}
	return noise;
}
