#include "BlendPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "Dragon.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

_BlendPass::_BlendPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_BlendPass::~_BlendPass()
{
}

void _BlendPass::initV()
{
	m_pShader = std::make_shared<_Shader>("BlendPass_VS.glsl", "BlendPass_FS.glsl");

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_ListHeadPtrTex", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("HeadTexture"));
}


void _BlendPass::updateV()
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