#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "EncodePass.h"


EncodePass::EncodePass(const std::string& vPassName, int vExcutionOrder) :_RenderPass(vPassName, vExcutionOrder)
{
}

EncodePass::~EncodePass()
{
}

//************************************************************************************
//Function:
void EncodePass::initV()
{
	std::vector<std::string> faces
	{
		"negx.bmp",
		"posx.bmp",
		"posy.bmp",
		"negy.bmp",
		"posz.bmp",
		"negz.bmp"
	};
	m_pShader = std::make_shared<_Shader>("Encode_VS.glsl", "Encode_FS.glsl");
	auto Texture = std::make_shared<MyGraphics::STexture>();
	Texture->Type4WrapR = Texture->Type4WrapS = Texture->Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture->Type4MinFilter = Texture->Type4MagFilter = GL_LINEAR;
	Texture->InternalFormat = GL_RGB;
	Texture->ExternalFormat = GL_RGB;
	Texture->TextureType = MyGraphics::STexture::ETextureType::TextureCubeMap;
	std::string path = "../Textures/Skybox2/";
	std::vector<std::string> p;
	for (int i = 0; i < faces.size(); i++)
	{
		p.push_back((path + faces[i]));
	}
	loadCubeTextureFromFile(p, Texture);


	auto TextureConfig4 = std::make_shared<MyGraphics::STexture>();
	TextureConfig4->InternalFormat = GL_RGBA32F;
	TextureConfig4->ExternalFormat = GL_RGBA;
	TextureConfig4->DataType = GL_FLOAT;
	TextureConfig4->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureConfig4->Type4WrapT = GL_CLAMP_TO_EDGE;
	TextureConfig4->Type4WrapR = GL_CLAMP_TO_EDGE;
	genTexture(TextureConfig4);

	m_FBO = genFBO({ TextureConfig4 });
	MyGraphics::ResourceManager::registerSharedData("EncodeTexture", TextureConfig4);


	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_Skybox", Texture);
}


//************************************************************************************
//Function:
void EncodePass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	m_pShader->activeShader();
	drawQuad();
	glDepthFunc(GL_LESS);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}