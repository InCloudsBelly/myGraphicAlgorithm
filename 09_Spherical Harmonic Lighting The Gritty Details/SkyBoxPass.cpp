#include "Shader.h"
#include "Sponza.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "SkyboxPass.h"


_SkyboxPass::_SkyboxPass(const std::string& vPassName, int vExcutionOrder) :_RenderPass(vPassName, vExcutionOrder)
{
}

_SkyboxPass::~_SkyboxPass()
{
}

//************************************************************************************
//Function:
void _SkyboxPass::initV()
{
	std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	MyGraphics::Camera::setMainCameraMoveSpeed(3);
	m_pShader = std::make_shared<_Shader>("Skybox_VS.glsl", "Skybox_FS.glsl");
	auto Texture = std::make_shared<MyGraphics::STexture>();
	Texture->Type4WrapR = Texture->Type4WrapS = Texture->Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture->Type4MinFilter = Texture->Type4MagFilter = GL_LINEAR;
	Texture->InternalFormat = GL_RGB;
	Texture->ExternalFormat = GL_RGB;
	Texture->TextureType = MyGraphics::STexture::ETextureType::TextureCubeMap;
	std::string path = "../Textures/Skybox/";
	std::vector<std::string> p;
	for (int i = 0; i < faces.size(); i++)
	{
		p.push_back((path + faces[i]));
	}
	loadCubeTextureFromFile(p, Texture);
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_Skybox", Texture);
}


//************************************************************************************
//Function:
void _SkyboxPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	m_pShader->activeShader();
	drawCube();
	glDepthFunc(GL_LESS);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}