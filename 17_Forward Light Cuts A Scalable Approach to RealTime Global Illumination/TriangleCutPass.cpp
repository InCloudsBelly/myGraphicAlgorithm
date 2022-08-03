#include "TriangleCutPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <random>

_TriangleCutPass::_TriangleCutPass(const std::string& vPassName, int vExcutionOrder) : _RenderPass(vPassName, vExcutionOrder)
{
}

_TriangleCutPass::~_TriangleCutPass()
{
}

float *_TriangleCutPass::getSk()
{
	float PI = glm::pi<float>();
	float *Sk = new float[m_NumS];
	Sk[0] = 4 * PI * m_D * m_D / m_N;
	float u = m_u;
	for (int i = 1; i < m_NumS; i++)
	{
		Sk[i] = Sk[0] * u;
		u *= m_u;
	}
	
	return Sk;
}
float *_TriangleCutPass::getSkp(float *Sk)
{
	float *Skp = new float[m_NumS];
	Skp[0] = Sk[0];
	for (int i = 1; i < m_NumS; i++)
	{
		float sum = 0;
		for (int j = 0; j < i; j++)
		{
			sum += 1.0 / Sk[j];
		}
		Skp[i] = 1.0 / sum;
	}
	return Skp;
}

float *_TriangleCutPass::generateNoise()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // Ëæ»ú¸¡µãÊý£¬·¶Î§0.0 - 1.0
	std::default_random_engine generator;
	float *ssaoNoise = new float[m_NoizeNum];
	for (int i = 0; i < m_NoizeNum; i++)
		ssaoNoise[i] = randomFloats(generator);
	return ssaoNoise;
}

void _TriangleCutPass::initV()
{
	float *Sk = getSk();
	float *Skp = getSkp(Sk);
	float *Noise = generateNoise();
	m_pSponza = std::dynamic_pointer_cast<_Sponza>(MyGraphics::ResourceManager::getGameObjectByName("Sponza"));
	
	m_TriangleInfoBuffer = genBuffer(GL_SHADER_STORAGE_BUFFER, 300000 * m_NumS * sizeof(float) * 12, NULL, GL_STATIC_DRAW,0);
	m_SkSSBO = genBuffer(GL_SHADER_STORAGE_BUFFER, m_NumS * sizeof(float), Sk, GL_STATIC_DRAW,1);
	m_NoiseSSBO = genBuffer(GL_SHADER_STORAGE_BUFFER, m_NoizeNum * sizeof(float), Noise, GL_STATIC_DRAW,2);
	m_SkpSSBO = genBuffer(GL_SHADER_STORAGE_BUFFER, m_NumS * sizeof(float), Skp, GL_STATIC_DRAW,3);
	m_AtomicSSBO = genBuffer(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), {0}, GL_DYNAMIC_COPY, 0);

	auto TextureDebug = std::make_shared<MyGraphics::STexture>();
	TextureDebug->InternalFormat = GL_RGBA32F;
	TextureDebug->ExternalFormat = GL_RGBA;
	TextureDebug->DataType= GL_FLOAT;
	genTexture(TextureDebug);
	m_FBO = genFBO({TextureDebug});


	m_pShader = std::make_shared<_Shader>("TriangleCutPass_VS.glsl", "TriangleCutPass_FS.glsl", "TriangleCutPass_GS.glsl");
	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setIntUniformValue("u_NoiseNum", m_NoizeNum);
	m_pShader->setIntUniformValue("u_NumS", m_NumS);
	m_pShader->setTextureUniformValue("u_LightDepthTexture", MyGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<MyGraphics::STexture>>("LightDepthTexture"));
	glm::vec3 LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	m_pSponza->initModel(*m_pShader);

	MyGraphics::ResourceManager::registerSharedData("DebugTexture", TextureDebug);
}

void _TriangleCutPass::clearAtomic()
{
	GLuint *userCounters;
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AtomicSSBO);
	userCounters = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER,
		0,
		sizeof(GLuint),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT
	);
	memset(userCounters, 0, sizeof(GLuint));
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
}


void _TriangleCutPass::updateV()
{
	clearAtomic();
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_pShader->activeShader();
	glm::mat4 m_LightVPMatrix = MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix") * MyGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(m_LightVPMatrix));
	auto LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);

	m_pSponza->updateModel(*m_pShader);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}