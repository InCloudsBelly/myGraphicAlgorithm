#include "LightSourcePass.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Interface.h"
#include "GameObject.h"
#include "Utils.h"
#include "CalculatePlanePass.h"
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

_LightSourcePass::_LightSourcePass(const std::string& vPassName, int vExcutionOrder) :_RenderPass(vPassName, vExcutionOrder)
{
}

_LightSourcePass::~_LightSourcePass()
{
}

//************************************************************************************
//Function:
void _LightSourcePass::initV()
{
	m_LightSourceTexture = std::make_shared<MyGraphics::STexture>();
	m_pShader = std::make_shared<_Shader>("LightSource_VS.glsl", "LightSource_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_tttttt", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("WeightAlbedoImage"));

}

//************************************************************************************
//Function:
int squrd = 0;
void _LightSourcePass::updateV()
{
	
		int ssbo = MyGraphics::ResourceManager::getSharedDataByName<int>("PlaneSSBO");
		int count = MyGraphics::ResourceManager::getSharedDataByName<int>("PlaneCountSSBO");
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		SPlane* plane = (SPlane*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, count);
		int* num = (int*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		int size = *num;
		int block = MyGraphics::ResourceManager::getSharedDataByName<int>("Block");
		for (int i = 0; i < block * block; i++, plane++)
		{
			float vertices[] = {
				plane->P0.x, plane->P0.y, plane->P0.z,  1.0f, 1.0f,
				plane->P1.x, plane->P1.y, plane->P1.z,  1.0f, 0.0f,
				plane->P2.x, plane->P2.y, plane->P2.z,  0.0f, 0.0f,
				plane->P2.x, plane->P2.y, plane->P2.z,  0.0f, 0.0f,
				plane->P3.x, plane->P3.y, plane->P3.z,  0.0f, 1.0f,
				plane->P0.x, plane->P0.y, plane->P0.z,  1.0f, 1.0f,
			};
			unsigned int VAO, VBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glEnable(GL_DEPTH_TEST);
			m_pShader->activeShader();
			m_pShader->setFloatUniformValue("u_Proportion", plane->SizeAndUV.x, plane->SizeAndUV.y);
			m_pShader->setFloatUniformValue("u_UV", plane->SizeAndUV.z, plane->SizeAndUV.w);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
}