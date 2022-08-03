//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "Model.h"
#include <iostream>
#include "Utils.h"
#include "common.h"
#include "Shader.h"
#include "AABB.h"

_Model::_Model(const std::string &vModelPath)
{
	__loadModel(vModelPath);
}

//************************************************************************************
//Function:
void _Model::init(_Shader &vioShader)
{
	vioShader.setCommonUniformAndId(DIFFUSE_TEX, 0);
	vioShader.setCommonUniformAndId(SPECULAR_TEX, 1);
	vioShader.setCommonUniformAndId(NORMAL_TEX, 2);
	vioShader.setCommonUniformAndId(ROUGHNESS_TEX, 3);
	vioShader.setCommonUniformAndId(METALLIC_TEX, 4);
	vioShader.setCommonUniformAndId(AMBIENT_COLOR);
	vioShader.setCommonUniformAndId(DIFFUSE_COLOR);
	vioShader.setCommonUniformAndId(SPECULAR_COLOR);
	vioShader.setCommonUniformAndId(MESH_ID);
	vioShader.setCommonUniformAndId(SHININESS);
	vioShader.setCommonUniformAndId(REFRACTI);

	for (auto &vMesh : m_Meshes)
		vMesh.init(vioShader);
}

//************************************************************************************
//Function:
GLvoid _Model::update(const _Shader& vShader) const
{
	for (auto &vMesh : m_Meshes)
		vMesh.update(vShader);
}

//************************************************************************************
//Function:
GLvoid _Model::__loadModel(const std::string& vPath)
{
	Assimp::Importer ModelImpoter;
	m_pScene = ModelImpoter.ReadFile(vPath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (!m_pScene || !m_pScene->mRootNode || m_pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cerr << "Error::Model:: " << ModelImpoter.GetErrorString() << std::endl;
		return;
	}
	m_Directory = vPath.substr(0, vPath.find_last_of('/'));
	__traverseNodes();
}

//************************************************************************************
//Function:
GLvoid _Model::__traverseNodes()
{
	GLint NumMeshes = static_cast<GLint>(m_pScene->mNumMeshes);
	for (GLint i = 0; i < NumMeshes; ++i)
		m_Meshes.push_back(__processMesh(m_pScene->mMeshes[i]));
}

//************************************************************************************
//Function:
_Mesh _Model::__processMesh(const aiMesh *vAiMesh)
{
	_ASSERT(vAiMesh);
	std::vector<_MeshVertex> Vertices;
	std::vector<GLint> Indices;
	std::vector<_MeshTexture> Textures;
	_MeshMatProperties MeshMatProperties;
	__processVertex(vAiMesh, Vertices);
	__processIndices(vAiMesh, Indices);
	__processTextures(vAiMesh, Textures);
	__processMatProperties(vAiMesh, MeshMatProperties);
	return _Mesh(Vertices, Indices, Textures, MeshMatProperties, ++m_MeshCount);
}

GLvoid _Model::__processMatProperties(const aiMesh *vAiMesh, _MeshMatProperties& voMeshMatProperties)
{
	_ASSERT(vAiMesh);
	if (vAiMesh->mMaterialIndex < 0)
		return;
	aiMaterial *pAiMat = m_pScene->mMaterials[vAiMesh->mMaterialIndex];
	aiColor3D AmbientColor, DiffuseColor, SpecularColor;
	float Shininess = 0.0f, Refracti = 0.0f;
	pAiMat->Get(AI_MATKEY_COLOR_AMBIENT, AmbientColor);
	pAiMat->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
	pAiMat->Get(AI_MATKEY_COLOR_SPECULAR, SpecularColor);
	pAiMat->Get(AI_MATKEY_SHININESS, Shininess);
	pAiMat->Get(AI_MATKEY_REFRACTI, Refracti);
	voMeshMatProperties.AmbientColor = { AmbientColor.r, AmbientColor.g, AmbientColor.b };
	voMeshMatProperties.DiffuseColor = { DiffuseColor.r, DiffuseColor.g, DiffuseColor.b };
	voMeshMatProperties.SpecularColor = { SpecularColor.r, SpecularColor.g, SpecularColor.b };
	voMeshMatProperties.Shininess = Shininess;
	voMeshMatProperties.Refracti = Refracti;
}

//************************************************************************************
//Function:
GLvoid _Model::__processVertex(const aiMesh *vAiMesh, std::vector<_MeshVertex> &voVertices)
{
	_ASSERT(vAiMesh);
	GLint NumVertices = (GLint)vAiMesh->mNumVertices;
	for (GLint i = 0; i < NumVertices; ++i)
	{
		_MeshVertex Vertex;
		Vertex.Position = glm::vec3(vAiMesh->mVertices[i].x, vAiMesh->mVertices[i].y, vAiMesh->mVertices[i].z);
		if (vAiMesh->mNormals != nullptr)
			Vertex.Normal = glm::vec3(vAiMesh->mNormals[i].x, vAiMesh->mNormals[i].y, vAiMesh->mNormals[i].z);
		else
			Vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		if (vAiMesh->mTextureCoords[0]) {
			Vertex.TexCoords = glm::vec2(vAiMesh->mTextureCoords[0][i].x, vAiMesh->mTextureCoords[0][i].y);
		}
		else {
			Vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		if (vAiMesh->mTangents)
			Vertex.Tangent = glm::vec3(vAiMesh->mTangents[i].x, vAiMesh->mTangents[i].y, vAiMesh->mTangents[i].z);
		
		voVertices.push_back(Vertex);
	}
}

//************************************************************************************
//Function:
GLvoid _Model::__processIndices(const aiMesh *vAiMesh, std::vector<GLint> &voIndices)
{
	_ASSERT(vAiMesh);
	GLint NumFaces = vAiMesh->mNumFaces;
	for (GLint i = 0; i < NumFaces; ++i)
	{
		aiFace AiFace = vAiMesh->mFaces[i];
		GLint NumIndices = AiFace.mNumIndices;
		for (GLint k = 0; k < NumIndices; ++k)
			voIndices.push_back(AiFace.mIndices[k]);
	}
}

//************************************************************************************
//Function:
GLvoid _Model::__processTextures(const aiMesh *vAiMesh, std::vector<_MeshTexture> &voTextures)
{
	_ASSERT(vAiMesh);
	if (vAiMesh->mMaterialIndex < 0)
		return;
	aiMaterial *pAiMat = m_pScene->mMaterials[vAiMesh->mMaterialIndex];
	MyGraphics::STexture Texture2DInSRGBSpace;
	Texture2DInSRGBSpace.isSRGBSpace = GL_TRUE;
	MyGraphics::STexture Texture2D4Roughness;
	Texture2D4Roughness.Type4MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	__loadTextureFromMaterial(aiTextureType_DIFFUSE, pAiMat, DIFFUSE_TEX, voTextures/*, Texture2DInSRGBSpace*/);
	__loadTextureFromMaterial(aiTextureType_SPECULAR, pAiMat, SPECULAR_TEX, voTextures);
	__loadTextureFromMaterial(aiTextureType_HEIGHT, pAiMat, NORMAL_TEX, voTextures);
	__loadTextureFromMaterial(aiTextureType_SHININESS, pAiMat, ROUGHNESS_TEX, voTextures/*, Texture2D4Roughness*/);
	__loadTextureFromMaterial(aiTextureType_AMBIENT, pAiMat, METALLIC_TEX, voTextures);
}

//************************************************************************************
//Function:
GLvoid _Model::__loadTextureFromMaterial(aiTextureType vTextureType, const aiMaterial *vMat, const std::string& vTextureNamePrefix, std::vector<_MeshTexture>& voTextures, std::shared_ptr<MyGraphics::STexture> vTexture2D/* = std::make_shared<MyGraphics::STexture>()*/)
{
	_ASSERT(vMat);
	GLint TextureCount = vMat->GetTextureCount(vTextureType);
	int TextureIndex = -1;
	if (TextureCount <= 0)
	{
		_MeshTexture MeshTexture;		//Note������ط�����ע�͵�ֱ��return����ΪҪ��init���õ�uniform��Ӧ
		voTextures.push_back(MeshTexture);
	}
	for (GLint i = 0; i < TextureCount; ++i)
	{
		aiString Str;
		vMat->GetTexture(vTextureType, i, &Str);
		std::string TextureName = Str.C_Str();
		std::string TexturePath = m_Directory + "/" + TextureName;
		GLboolean Skip = GL_FALSE;
		GLint LoadedTextureCount = static_cast<int>(m_LoadedTextures.size());
		for (int k = 0; k < LoadedTextureCount; ++k)
		{
			if (TexturePath == m_LoadedTextures[k].TexturePath)
			{
				Skip = GL_TRUE;
				m_LoadedTextures[k].TextureUniformName = vTextureNamePrefix + std::to_string(++TextureIndex);
				voTextures.push_back(m_LoadedTextures[k]);
				break;
			}
		}
		if (!Skip)
		{
			_MeshTexture MeshTexture;
			loadTextureFromFile(TexturePath, vTexture2D);
			MeshTexture.ID = vTexture2D->TextureID;
			MeshTexture.TexturePath = TexturePath;
			MeshTexture.TextureUniformName = vTextureNamePrefix + std::to_string(++TextureIndex);
			voTextures.push_back(MeshTexture);
			m_LoadedTextures.push_back(MeshTexture);
		}
	}
}

//************************************************************************************
//Function:
std::shared_ptr<_AABB> _Model::getOrCreateBounding()
{
	if (!m_pBounding)
	{
		m_pBounding = std::make_shared<_AABB>();
		size_t MeshSize = m_Meshes.size();
		for (size_t i = 0; i < MeshSize; ++i)
		{
			m_pBounding->combine(*m_Meshes[i].getOrCreateBounding());
		}
	}
	return m_pBounding;
}

//************************************************************************************
//Function:
std::vector<glm::vec3> _Model::getTriangle()
{
	std::vector<glm::vec3> Triangle;
	size_t MeshSize = m_Meshes.size();
	for (size_t i = 0; i < MeshSize; ++i)
	{
		std::vector<glm::vec3> temp = m_Meshes[i].getTriangle();
		Triangle.insert(Triangle.end(), temp.begin(), temp.end());
	}
	return Triangle;
}