//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

#define MESH_ID			"u_MeshId"
#define SHININESS		"u_Shininess"
#define REFRACTI		"u_Refracti"
#define AMBIENT_COLOR	"u_AmbientColor"
#define DIFFUSE_COLOR	"u_DiffuseColor"
#define SPECULAR_COLOR	"u_SpecularColor"
#define DIFFUSE_TEX		"u_DiffuseTexture"
#define SPECULAR_TEX	"u_SpecularTexture"
#define NORMAL_TEX		"u_NormalTexture"
#define ROUGHNESS_TEX	"u_RoughnessTexture"
#define METALLIC_TEX	"u_MetallicTexture"

class _Shader;
class _AABB;

struct _MeshVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;

	_MeshVertex() {}
};

struct _MeshTexture
{
	GLint ID = -1;
	std::string TexturePath;
	std::string TextureUniformName;

	_MeshTexture() {}
};

struct _MeshMatProperties
{
	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;
	float Shininess = 0.0f;
	float Refracti = 0.0f;
};

class _Mesh
{
public:
	_Mesh() = default;
	_Mesh(const std::vector<_MeshVertex>& vVertices, const std::vector<GLint>& vIndices, const std::vector<_MeshTexture>& vTexture, const _MeshMatProperties& vMeshMatProperties, int vMeshId);
	~_Mesh();
	GLvoid init(const _Shader& vioShader);
	GLvoid update(const _Shader& vShader) const;

	std::shared_ptr<_AABB> getOrCreateBounding();
	std::vector<glm::vec3> getTriangle();
private:
	std::vector<_MeshVertex>	m_Vertices;
	std::vector<_MeshTexture>	m_Textures;
	std::vector<GLint>			m_Indices;
	std::shared_ptr<_AABB>		m_pBounding;
	_MeshMatProperties			m_MeshMatProperties;
	GLint m_VAO = -1;
	GLint m_AmbientColorLoc = -1;
	GLint m_DiffuseColorLoc = -1;
	GLint m_SpecularColorLoc = -1;
	GLint m_MeshId = -1;

	GLvoid __init();
};