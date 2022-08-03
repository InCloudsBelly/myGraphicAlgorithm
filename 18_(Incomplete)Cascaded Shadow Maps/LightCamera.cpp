#include "LightCamera.h"
#include "Interface.h"
#include <glm/gtc/matrix_transform.hpp>
#include "iostream"
_LightCamera::_LightCamera(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

_LightCamera::~_LightCamera()
{
}
void _LightCamera::initFrustum()
{
	m_FrustumNF.resize(m_SplitNum);
	m_Frustum.resize(m_SplitNum);
}


void _LightCamera::setFrustum()
{
	glm::vec3 Center = MyGraphics::Camera::getMainCameraPos();
	glm::vec3 Direct = MyGraphics::Camera::getMainCameraDir();
	glm::vec3 Up(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = glm::normalize(glm::cross(Direct, Up));

	float FOVHalf = MyGraphics::Camera::getMainCameraFov();
	float Ratio = MyGraphics::WINDOW_KEYWORD::getWindowHeight() / MyGraphics::WINDOW_KEYWORD::getWindowWidth();
	float Near = MyGraphics::Camera::getMainCameraNear();
	float Far = MyGraphics::Camera::getMainCameraFar();

	float Lamuda = Far / Near;
	m_FrustumNF[0].Near = Near;
	for (int i = 1; i < m_SplitNum; i++) {
		float Si = i / (float)m_SplitNum;
		float t_Near = m_Weight * (Near * powf(Lamuda, Si)) + (1 - m_Weight) * (Near + (Far - Near) * Si);
		float t_Far = t_Near;
		m_FrustumNF[i].Near = t_Near;
		m_FrustumNF[i - 1].Far = t_Far;
	}
	m_FrustumNF[m_SplitNum - 1].Far = Far;

	for (int i = 0; i < m_SplitNum; i++) {
		FrustumPos& t_frustum = m_FrustumNF[i];

		glm::vec3 fc = Center + Direct * t_frustum.Far;
		glm::vec3 nc = Center + Direct * t_frustum.Near;

		Right = glm::normalize(Right);
		Up = glm::normalize(glm::cross(Right, Direct));

		float NearHeight = glm::tan(glm::radians(FOVHalf)/2) * t_frustum.Near;
		float NearWidth = NearHeight * Ratio;
		float FarHeight = glm::tan(glm::radians(FOVHalf)/2) * t_frustum.Far;
		float FarWidth = FarHeight * Ratio;

		t_frustum.Pos.push_back(nc - Up * NearHeight - Right * NearWidth);
		t_frustum.Pos.push_back(nc + Up * NearHeight - Right * NearWidth);
		t_frustum.Pos.push_back(nc + Up * NearHeight + Right * NearWidth);
		t_frustum.Pos.push_back(nc - Up * NearHeight + Right * NearWidth);

		t_frustum.Pos.push_back(fc - Up * FarHeight - Right * FarWidth);
		t_frustum.Pos.push_back(fc + Up * FarHeight - Right * FarWidth);
		t_frustum.Pos.push_back(fc + Up * FarHeight + Right * FarWidth);
		t_frustum.Pos.push_back(fc - Up * FarHeight + Right * FarWidth);

		glm::vec3 tmax(-1000.0f, -1000.0f, -1000.0f);
		glm::vec3 tmin(1000.0f, 1000.0f, 1000.0f);

		for (int j = 0; j < 8; j++) {
			glm::vec4 t_transf = m_LightViewMatrix * glm::vec4(t_frustum.Pos[j], 1.0f);
			if (t_transf.z > tmax.z) { tmax.z = t_transf.z; }
			if (t_transf.z < tmin.z) { tmin.z = t_transf.z; }
		}
		tmax.z += 10.0f;
		glm::mat4 t_ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -tmax.z, -tmin.z);

		for (int j = 0; j < 8; j++) {
			glm::vec4 t_transf = t_ortho * m_LightViewMatrix * glm::vec4(t_frustum.Pos[j], 1.0f);
			t_transf.x /= t_transf.w;
			t_transf.y /= t_transf.w;

			if (t_transf.x > tmax.x) { tmax.x = t_transf.x; }
			if (t_transf.x < tmin.x) { tmin.x = t_transf.x; }
			if (t_transf.y > tmax.y) { tmax.y = t_transf.y; }
			if (t_transf.y < tmin.y) { tmin.y = t_transf.y; }
		}

		glm::vec2 tscale(2.0f / (tmax.x - tmin.x), 2.0f / (tmax.y - tmin.y));
		glm::vec2 toffset(-0.5f * (tmax.x + tmin.x) * tscale.x, -0.5f * (tmax.y + tmin.y) * tscale.y);
		glm::mat4 t_shad_crop = glm::mat4(1);

		t_shad_crop[0][0] = tscale.x;
		t_shad_crop[1][1] = tscale.y;
		t_shad_crop[0][3] = toffset.x;
		t_shad_crop[1][3] = toffset.y;
		t_shad_crop = glm::transpose(t_shad_crop);
		m_Frustum[i] = t_shad_crop * t_ortho;
		m_crop_matrices[i] = m_Frustum[i] * m_LightViewMatrix;
	}
}
void _LightCamera::initV()
{
	initFrustum();
	MyGraphics::Camera::setMainCameraFarPlane(150.0f);
	MyGraphics::Camera::setMainCameraPos({ 0, 39.1055, 62.8284 });
	MyGraphics::Camera::setMainCameraFront({ 0, -0, -1});
	MyGraphics::Camera::setMainCameraMoveSpeed(15);
	m_LightPos = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightPos");
	m_LightDir = MyGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	if (abs(m_LightDir) == m_LightUpVector)
		m_LightDir.z += 0.01f;
	m_LightViewMatrix = glm::lookAt(glm::vec3(0), m_LightDir, m_LightUpVector);
	auto m_LightProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	MyGraphics::ResourceManager::registerSharedData("LightViewMatrix", m_LightViewMatrix);
	MyGraphics::ResourceManager::registerSharedData("LightProjectionMatrix", m_LightProjectionMatrix);
	MyGraphics::ResourceManager::registerSharedData("Exposure", m_Exposure);
	MyGraphics::ResourceManager::registerSharedData("SplitNum", m_SplitNum);
	setFrustum();



	glm::mat4 t_view = MyGraphics::Camera::getMainCameraViewMatrix();
    glm::mat4 t_view_inverse = glm::inverse(t_view);
    glm::mat4 t_projection = MyGraphics::Camera::getMainCameraProjectionMatrix();
	std::vector<float> m_far_bounds;
	for (int i = 0; i < m_SplitNum; i++) {
		m_far_bounds.push_back(0.5f * ((1.0f * m_FrustumNF[i].Far * t_projection[2][2] + t_projection[3][2]) /
			-m_FrustumNF[i].Far) + 0.5f);

		MyGraphics::ResourceManager::registerSharedData("FarPlane[" + std::to_string(i) + "]", m_far_bounds[i]);
	}
}

void _LightCamera::updateV()
{
}