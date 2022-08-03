#include "MergeShadingPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CMergeShadingPass::CMergeShadingPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

CMergeShadingPass::~CMergeShadingPass()
{
}

void CMergeShadingPass::initV()
{
	m_pShader = std::make_shared<_Shader>("MergeShading_VS.glsl", "MergeShading_FS.glsl");

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_OpacityAlbedoTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("OpacityAlbedoTexture"));
	m_pShader->setTextureUniformValue("u_TansparentTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("TansparentTexture"));
	m_pShader->setTextureUniformValue("u_CoefficientOneTex", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("CoefficientOne"));
}


void CMergeShadingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	m_pShader->activeShader();
	drawQuad();
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}