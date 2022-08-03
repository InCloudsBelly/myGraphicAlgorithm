#include "PropagationPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
CPropagationPass::CPropagationPass(const std::string& vPassName, int vExecutionOrder) : _RenderPass(vPassName, vExecutionOrder)
{
}

CPropagationPass::~CPropagationPass()
{
}

void CPropagationPass::drawVPLVAO()
{

	glm::vec3 MinAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	glm::vec3 MaxAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	if (m_VPLVAO == -1)
	{
		std::vector<glm::vec3> Vertices;
		for (int i = 0; i < m_Dimensions.x; i++)
			for (int j = 0; j < m_Dimensions.y; j++)
				for (int k = 0; k < m_Dimensions.z; k++)
					Vertices.push_back(glm::vec3(i, j, k));

		m_VPLVAO = createVAO(&Vertices.front(), Vertices.size() * 3 * sizeof(float), {3});
	}
	glBindVertexArray(m_VPLVAO);
	glDrawArrays(GL_POINTS, 0, m_Dimensions.x * m_Dimensions.y * m_Dimensions.z);
	glBindVertexArray(0);
}


//************************************************************************************
//Function:
void CPropagationPass::initV()
{
	int RSMResolution = MyGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	glm::vec3 MinAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	glm::vec3 MaxAABB = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");

	m_Dimensions = MaxAABB - MinAABB;
	m_TextureLPVGridR.push_back(MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVGridRTexture"));
	m_TextureLPVGridG.push_back(MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVGridGTexture"));
	m_TextureLPVGridB.push_back(MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVGridBTexture"));
	for (int i = 1; i <= m_ProgationNum; i++)
	{
		auto TextureLPVGridR = std::make_shared<MyGraphics::STexture>();
		auto TextureLPVGridG = std::make_shared<MyGraphics::STexture>();
		auto TextureLPVGridB = std::make_shared<MyGraphics::STexture>();
		TextureLPVGridR->ImageBindUnit = 3;
		TextureLPVGridG->ImageBindUnit = 4;
		TextureLPVGridB->ImageBindUnit = 5;
		TextureLPVGridR->TextureType = TextureLPVGridG->TextureType = TextureLPVGridB->TextureType = MyGraphics::STexture::ETextureType::Texture3D;
		TextureLPVGridR->InternalFormat = TextureLPVGridG->InternalFormat = TextureLPVGridB->InternalFormat = GL_RGBA16F;
		TextureLPVGridR->ExternalFormat = TextureLPVGridG->ExternalFormat = TextureLPVGridB->ExternalFormat = GL_RGBA;
		TextureLPVGridR->DataType = TextureLPVGridG->DataType = TextureLPVGridB->DataType = GL_FLOAT;
		TextureLPVGridR->Width = TextureLPVGridG->Width = TextureLPVGridB->Width = m_Dimensions.x;
		TextureLPVGridR->Height = TextureLPVGridG->Height = TextureLPVGridB->Height = m_Dimensions.y;
		TextureLPVGridR->Depth = TextureLPVGridG->Depth = TextureLPVGridB->Depth = m_Dimensions.z;
		TextureLPVGridR->Type4WrapS = TextureLPVGridG->Type4WrapS = TextureLPVGridB->Type4WrapS = GL_CLAMP_TO_EDGE;
		TextureLPVGridR->Type4WrapT = TextureLPVGridG->Type4WrapT = TextureLPVGridB->Type4WrapT = GL_CLAMP_TO_EDGE;

		genTexture(TextureLPVGridR);
		genTexture(TextureLPVGridG);
		genTexture(TextureLPVGridB);

		m_TextureLPVGridR.push_back(TextureLPVGridR);
		m_TextureLPVGridG.push_back(TextureLPVGridG);
		m_TextureLPVGridB.push_back(TextureLPVGridB);
	}
	
	m_TextureLPVAccumulatorR = std::make_shared<MyGraphics::STexture>();
	m_TextureLPVAccumulatorG = std::make_shared<MyGraphics::STexture>();
	m_TextureLPVAccumulatorB = std::make_shared<MyGraphics::STexture>();
	m_TextureLPVAccumulatorR->ImageBindUnit = 0;
	m_TextureLPVAccumulatorG->ImageBindUnit = 1;
	m_TextureLPVAccumulatorB->ImageBindUnit = 2;
	m_TextureLPVAccumulatorR->TextureType = m_TextureLPVAccumulatorG->TextureType = m_TextureLPVAccumulatorB->TextureType = MyGraphics::STexture::ETextureType::Texture3D;
	m_TextureLPVAccumulatorR->InternalFormat = m_TextureLPVAccumulatorG->InternalFormat = m_TextureLPVAccumulatorB->InternalFormat = GL_RGBA16F;
	m_TextureLPVAccumulatorR->ExternalFormat = m_TextureLPVAccumulatorG->ExternalFormat = m_TextureLPVAccumulatorB->ExternalFormat = GL_RGBA;
	m_TextureLPVAccumulatorR->DataType = m_TextureLPVAccumulatorG->DataType = m_TextureLPVAccumulatorB->DataType = GL_FLOAT;
	m_TextureLPVAccumulatorR->Width = m_TextureLPVAccumulatorG->Width = m_TextureLPVAccumulatorB->Width = m_Dimensions.x;
	m_TextureLPVAccumulatorR->Height = m_TextureLPVAccumulatorG->Height = m_TextureLPVAccumulatorB->Height = m_Dimensions.y;
	m_TextureLPVAccumulatorR->Depth = m_TextureLPVAccumulatorG->Depth = m_TextureLPVAccumulatorB->Depth = m_Dimensions.z;
	m_TextureLPVAccumulatorR->Type4WrapS = m_TextureLPVAccumulatorG->Type4WrapS = m_TextureLPVAccumulatorB->Type4WrapS = GL_CLAMP_TO_EDGE;
	m_TextureLPVAccumulatorR->Type4WrapT = m_TextureLPVAccumulatorG->Type4WrapT = m_TextureLPVAccumulatorB->Type4WrapT = GL_CLAMP_TO_EDGE;

	genTexture(m_TextureLPVAccumulatorR);
	genTexture(m_TextureLPVAccumulatorG);
	genTexture(m_TextureLPVAccumulatorB);




	m_pShader = std::make_shared<_Shader>("PropagationPass_VS.glsl", "PropagationPass_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_GridDim", m_Dimensions.x, m_Dimensions.y, m_Dimensions.z);
	m_pShader->setIntUniformValue("u_FirstPropStep", false);
	m_pShader->setTextureUniformValue("LPVGridR", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVGridRTexture"));
	m_pShader->setTextureUniformValue("LPVGridG", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVGridGTexture"));
	m_pShader->setTextureUniformValue("LPVGridB", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVGridBTexture"));
	m_pShader->setTextureUniformValue("GeometryVolume", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LPVGVTexture"));
	m_pShader->setImageUniformValue(m_TextureLPVAccumulatorR);
	m_pShader->setImageUniformValue(m_TextureLPVAccumulatorG);
	m_pShader->setImageUniformValue(m_TextureLPVAccumulatorB);

	MyGraphics::ResourceManager::registerSharedData("LPVAccumulatorRTexture", m_TextureLPVAccumulatorR);
	MyGraphics::ResourceManager::registerSharedData("LPVAccumulatorGTexture", m_TextureLPVAccumulatorG);
	MyGraphics::ResourceManager::registerSharedData("LPVAccumulatorBTexture", m_TextureLPVAccumulatorB);
}

//************************************************************************************
//Function:
void CPropagationPass::updateV()
{
	for (int i = 1; i <= m_ProgationNum; i++)
	{
		ClearTexture(m_TextureLPVGridR[i], GL_TEXTURE_3D);
		ClearTexture(m_TextureLPVGridG[i], GL_TEXTURE_3D);
		ClearTexture(m_TextureLPVGridB[i], GL_TEXTURE_3D);
	}
	ClearTexture(m_TextureLPVAccumulatorR, GL_TEXTURE_3D);
	ClearTexture(m_TextureLPVAccumulatorG, GL_TEXTURE_3D);
	ClearTexture(m_TextureLPVAccumulatorB, GL_TEXTURE_3D);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_Dimensions.x, m_Dimensions.y);
	m_pShader->activeShader();
	for (int i = 1; i <= m_ProgationNum; i++)
	{
		if (i == 0)
		{
			m_pShader->setIntUniformValue("u_FirstPropStep", true);
		}
		else
		{
			m_pShader->setIntUniformValue("u_FirstPropStep", false);
		}
		m_pShader->setTextureUniformValue("LPVGridR", m_TextureLPVGridR[i - 1]);
		m_pShader->setTextureUniformValue("LPVGridG", m_TextureLPVGridG[i - 1]);
		m_pShader->setTextureUniformValue("LPVGridB", m_TextureLPVGridB[i - 1]);
		glBindImageTexture(3, m_TextureLPVGridR[i]->TextureID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
		glBindImageTexture(4, m_TextureLPVGridG[i]->TextureID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
		glBindImageTexture(5, m_TextureLPVGridB[i]->TextureID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);

		drawVPLVAO();
	}
	glViewport(0, 0, MyGraphics::WINDOW_KEYWORD::getWindowWidth(), MyGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}