//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <memory>
#include "Mesh.h"
#include "Common.h"

class _Shader;
class _AABB;

class _Model
{
public:
	_Model() = default;
	_Model(const std::string &vModelPath);
	~_Model() = default;
	void init(_Shader &vioShader);
	void update(const _Shader &vShader) const;

	std::shared_ptr<_AABB> getOrCreateBounding();
	std::vector<glm::vec3> getTriangle();
private:
	int						   m_MeshCount = -1;
	std::vector<_MeshTexture>  m_LoadedTextures;
	std::vector<_Mesh>         m_Meshes;
	std::string                m_Directory;
	const aiScene             *m_pScene;
	std::shared_ptr<_AABB>     m_pBounding;	//这个地方的设计不好，应该由工厂模式来创建对应的包围盒

	GLvoid __loadModel(const std::string& vPath);
	GLvoid __traverseNodes();
	_Mesh  __processMesh(const aiMesh *vAiMesh);
	GLvoid __processVertex(const aiMesh *vAiMesh, std::vector<_MeshVertex> &voVertices);
	GLvoid __processIndices(const aiMesh *vAiMesh, std::vector<GLint> &voIndices);
	GLvoid __processTextures(const aiMesh *vAiMesh, std::vector<_MeshTexture> &voTextures);
	GLvoid __processMatProperties(const aiMesh *vAiMesh, _MeshMatProperties& voMeshMatProperties);
	GLvoid __loadTextureFromMaterial(aiTextureType vTextureType, const aiMaterial *vMat, const std::string& vTextureNamePrefix, std::vector<_MeshTexture>& voTextures, std::shared_ptr<MyGraphics::STexture> vTexture2D = std::make_shared<MyGraphics::STexture>());
};