#include "GroundPass.h"
#include "Common.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include "Utils.h"
#include "Shader.h"
#include "Interface.h"
#include "Ground.h"
_GroundPass::_GroundPass(const std::string& vPassName, int vExcutionOrder) :_RenderPass(vPassName, vExcutionOrder)
{
}

_GroundPass::~_GroundPass()
{
}

//************************************************************************************
//Function:
void _GroundPass::initV()
{
	
	
	m_pShader = std::make_shared<_Shader>("Ground_VS.glsl", "Ground_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_TileArrayTextures", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("TextureArray"));
	m_pShader->setIntUniformValue("u_TileWidth", MyGraphics::ResourceManager::getSharedDataByName<int>("TileWidth"));
	m_pShader->setIntUniformValue("u_TileHeight", MyGraphics::ResourceManager::getSharedDataByName<int>("TileHeight"));
	m_pShader->setIntUniformValue("u_WindowWidth", MyGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", MyGraphics::WINDOW_KEYWORD::getWindowHeight());
}

//************************************************************************************
//Function:
void _GroundPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}