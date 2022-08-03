#include "IndirectShadingPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_IndirectShadingPass::_IndirectShadingPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_IndirectShadingPass::~_IndirectShadingPass()
{
}

void _IndirectShadingPass::initV()
{
	
	m_pShader = std::make_shared<_Shader>("IndirectShading_VS.glsl", "IndirectShading_FS.glsl");
	auto TextureIndirectLight = std::make_shared<MyGraphics::STexture>();
	TextureIndirectLight->InternalFormat = GL_RGBA32F;
	TextureIndirectLight->ExternalFormat = GL_RGBA;
	TextureIndirectLight->DataType = GL_FLOAT;
	genTexture(TextureIndirectLight);
	m_FBO = genFBO({TextureIndirectLight});

	MyGraphics::ResourceManager::registerSharedData("IndirectLightTexture", TextureIndirectLight);

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputNormalTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_InputPositionTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("PositionTexture"));
	m_pShader->setFloatUniformValue("u_Intensity", MyGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));

}																

void _IndirectShadingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}