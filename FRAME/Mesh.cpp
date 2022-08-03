//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "Mesh.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include "AABB.h"

_Mesh::_Mesh(const std::vector<_MeshVertex>& vVertices, const std::vector<GLint>& vIndices, const std::vector<_MeshTexture>& vTexture, const _MeshMatProperties& vMeshMatProperties, int vMeshId)
	:m_Vertices(vVertices), m_Indices(vIndices), m_Textures(vTexture), m_MeshMatProperties(vMeshMatProperties), m_MeshId(vMeshId)
{
	__init();
}

_Mesh::~_Mesh()
{
}

//************************************************************************************
//Function:
GLvoid _Mesh::__init()
{
	m_VAO = createVAO(&m_Vertices[0], static_cast<int>(m_Vertices.size() * sizeof(_MeshVertex)), { 3,3,2,3 }, &m_Indices[0], static_cast<int>(m_Indices.size() * sizeof(GLint)));
}

//************************************************************************************
//Function:
GLvoid _Mesh::init(const _Shader& vioShader)
{
}

//************************************************************************************
//Function:
GLvoid _Mesh::update(const _Shader& vShader) const
{
	//_WARNING(m_Textures.size() > 5, "Texture num of some mesh is greater than 5.");
	if (m_Textures.size() > 0 && m_Textures[0].ID != -1)
	{
		for (int i = 0; i < m_Textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_Textures[i].ID);
		}
	}
	else
	{
		int AmbientColorLoc = vShader.getCommonUniformId(AMBIENT_COLOR);
		if (AmbientColorLoc >= 0)	//��Ʋ��㣬��ʵmesh������id�����ʲ�����Щ��Ӧ����һ��������Mat��װ����
			vShader.setFloatUniformValue(AmbientColorLoc, m_MeshMatProperties.AmbientColor.r, m_MeshMatProperties.AmbientColor.g, m_MeshMatProperties.AmbientColor.b);

		int DiffuseColorLoc = vShader.getCommonUniformId(DIFFUSE_COLOR);
		if (DiffuseColorLoc >= 0)
			vShader.setFloatUniformValue(DiffuseColorLoc, m_MeshMatProperties.DiffuseColor.r, m_MeshMatProperties.DiffuseColor.g, m_MeshMatProperties.DiffuseColor.b);

		int SpecularColorLoc = vShader.getCommonUniformId(SPECULAR_COLOR);
		if (SpecularColorLoc >= 0)
			vShader.setFloatUniformValue(SpecularColorLoc, m_MeshMatProperties.SpecularColor.r, m_MeshMatProperties.SpecularColor.g, m_MeshMatProperties.SpecularColor.b);
		
		int ShininessLoc = vShader.getCommonUniformId(SHININESS);
		if (ShininessLoc >= 0)
			vShader.setFloatUniformValue(ShininessLoc, m_MeshMatProperties.Shininess);

		int RefractiLoc = vShader.getCommonUniformId(REFRACTI);
		if (RefractiLoc >= 0)
			vShader.setFloatUniformValue(RefractiLoc, m_MeshMatProperties.Refracti);
	}

	int MeshIdLoc = vShader.getCommonUniformId(MESH_ID);
	if (MeshIdLoc)
		vShader.setIntUniformValue(MeshIdLoc, m_MeshId);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, static_cast<int>(m_Indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//************************************************************************************
//Function:
std::shared_ptr<_AABB> _Mesh::getOrCreateBounding()
{
	if (!m_pBounding)
	{
		std::vector<glm::vec3> Points;
		size_t VerticesNum = m_Vertices.size();
		for (size_t i = 0; i < VerticesNum; ++i)
		{
			Points.push_back(m_Vertices[i].Position);
		}
		m_pBounding = std::make_shared<_AABB>(Points);
	}
	return m_pBounding;
}

//************************************************************************************
//Function:
std::vector<glm::vec3> _Mesh::getTriangle()
{
	std::vector<glm::vec3> Triangle;
	size_t VerticesNum = m_Vertices.size();
	for (size_t i = 0; i < VerticesNum; ++i)
		Triangle.push_back(m_Vertices[i].Position);
	return Triangle;
}