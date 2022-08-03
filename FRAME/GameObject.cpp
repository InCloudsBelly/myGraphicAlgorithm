//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Model.h"
#include "Utils.h"
#include "Common.h"

_GameObject::_GameObject()
{
}

_GameObject::_GameObject(const std::string &vGameObjectName, int vExecutionOrder) : m_Name(vGameObjectName), m_ExecutionOrder(vExecutionOrder)
{
}

_GameObject::~_GameObject()
{
}

//************************************************************************************
//Function:
bool _GameObject::operator<(const _GameObject& vOtherPass) const
{
	return m_ExecutionOrder < vOtherPass.getExecutationOrder();
}

//************************************************************************************
//Function:
const glm::vec3& _GameObject::getPosition() const
{
	return m_Position;
}

//************************************************************************************
//Function:
float _GameObject::getRotationX() const
{
	return m_RotationAngle[0];
}

//************************************************************************************
//Function:
float _GameObject::getRotationY() const
{
	return m_RotationAngle[1];
}

//************************************************************************************
//Function:
float _GameObject::getRotationZ() const
{
	return m_RotationAngle[2];
}

//************************************************************************************
//Function:
const glm::vec3& _GameObject::getScale() const 
{
	return m_Scale;
}

//************************************************************************************
//Function:
void _GameObject::setPosition(const glm::vec3& vPosition)
{
	//m_ModelMatrix = glm::translate(m_ModelMatrix, vPosition - m_Position);
	//m_Position = vPosition;

	/*m_Position = vPosition;
	__updateModelMatrix();*/
	if (vPosition != m_Position)
	{
		m_TranslationMatrix = glm::mat4();
		m_TranslationMatrix = glm::translate(m_TranslationMatrix, vPosition);
		__updateModelMatrix();
		m_Position = vPosition;
	}
}

//************************************************************************************
//Function:
void _GameObject::__setRotation(float vRotationAngleOffset, const glm::vec3& vRotateAxis)
{
	if (!floatEqual(vRotationAngleOffset, 0.0))
	{
		glm::mat4 TempRotationMatrix;
		TempRotationMatrix = glm::rotate(TempRotationMatrix, glm::radians(vRotationAngleOffset), vRotateAxis);
		m_RotationMatrix = TempRotationMatrix * m_RotationMatrix;
		__updateModelMatrix();
		//m_RotationAngle[0] = vRotationAngle;
	}
}

//************************************************************************************
//Function:
void _GameObject::setRotationX(float vRotationAngle)
{
	////if (vRotationAngle[0])
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[0] - m_RotationAngle[0]), glm::vec3(1, 0, 0));
	////if (vRotationAngle[1])
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[1] - m_RotationAngle[1]), glm::vec3(0, 1, 0));
	////if (vRotationAngle[2])
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationAngle[2] - m_RotationAngle[2]), glm::vec3(0, 0, 1));
	//m_RotationAngle = vRotationAngle;

	/*m_RotationAngle = vRotationAngle;
	__updateModelMatrix();*/

	__setRotation(vRotationAngle - m_RotationAngle[0], glm::vec3(1, 0, 0));
	m_RotationAngle[0] = vRotationAngle;
}

//************************************************************************************
//Function:
void _GameObject::setRotationY(float vRotationAngle)
{
	__setRotation(vRotationAngle - m_RotationAngle[1], glm::vec3(0, 1, 0));
	m_RotationAngle[1] = vRotationAngle;
}

//************************************************************************************
//Function:
void _GameObject::setRotationZ(float vRotationAngle)
{
	__setRotation(vRotationAngle - m_RotationAngle[2], glm::vec3(0, 0, 1));
	m_RotationAngle[2] = vRotationAngle;
}

//************************************************************************************
//Function:
void _GameObject::setScale(const glm::vec3& vScale)
{
	/*m_ModelMatrix = glm::scale(m_ModelMatrix, vScale / m_Scale);
	m_Scale = vScale;*/
	/*m_Scale = vScale;
	__updateModelMatrix();*/

	if (vScale != m_Scale)
	{
		m_ScaleMatrix = glm::mat4();
		m_ScaleMatrix = glm::scale(m_ScaleMatrix, vScale);
		__updateModelMatrix();
		m_Scale = vScale;
	}
}

//************************************************************************************
//Function:
void _GameObject::translate(const glm::vec3& vPositionOffset)
{
	/*m_ModelMatrix = glm::translate(m_ModelMatrix, vPositionOffset);
	m_Position += vPositionOffset;*/

	/*m_Position += vPositionOffset;
	__updateModelMatrix();*/

	if (vPositionOffset != glm::vec3(0))
	{
		m_TranslationMatrix = glm::translate(m_TranslationMatrix, vPositionOffset);
		__updateModelMatrix();
		m_Position += vPositionOffset;
	}
}

//************************************************************************************
//Function:
void _GameObject::rotateX(float vRotationOffset)
{
	/*if (vRotationOffset[0])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[0]), glm::vec3(1, 0, 0));
	if (vRotationOffset[1])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[1]), glm::vec3(0, 1, 0));
	if (vRotationOffset[2])
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(vRotationOffset[2]), glm::vec3(0, 0, 1));
	m_RotationAngle += vRotationOffset;*/

	__setRotation(vRotationOffset, glm::vec3(1, 0, 0));
	m_RotationAngle[0] += vRotationOffset;
}

//************************************************************************************
//Function:
void _GameObject::rotateY(float vRotationOffset)
{
	__setRotation(vRotationOffset, glm::vec3(0, 1, 0));
	m_RotationAngle[1] += vRotationOffset;
}

//************************************************************************************
//Function:
void _GameObject::rotateZ(float vRotationOffset)
{
	__setRotation(vRotationOffset, glm::vec3(0, 0, 1));
	m_RotationAngle[2] += vRotationOffset;
}

//************************************************************************************
//Function:
void _GameObject::scale(const glm::vec3& vScaleOffset)
{
	/*m_ModelMatrix = glm::scale(m_ModelMatrix, vScaleOffset);
	m_Scale *= vScaleOffset;*/

	/*m_Scale *= vScaleOffset;
	__updateModelMatrix();*/

	if (vScaleOffset != glm::vec3(0))
	{
		m_ScaleMatrix = glm::scale(m_ScaleMatrix, vScaleOffset);
		__updateModelMatrix();
		m_Scale *= vScaleOffset;
	}
}

//************************************************************************************
//Function:
void _GameObject::__updateModelMatrix()
{
	m_ModelMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
}

//************************************************************************************
//Function:
const glm::mat4& _GameObject::getModelMatrix() const
{
	return m_ModelMatrix;
}

//************************************************************************************
//Function:
int _GameObject::getVAO() const 
{
	return m_VAO;
}

//************************************************************************************
//Function:
void _GameObject::initModel(_Shader& vioShader) const
{
	m_pModel->init(vioShader);
}

//************************************************************************************
//Function:
void _GameObject::updateModel(const _Shader& vShader) const
{
	m_pModel->update(vShader);
}

//************************************************************************************
//Function:
std::shared_ptr<_AABB> _GameObject::getAABB() const
{
	return m_pModel->getOrCreateBounding();
}

//************************************************************************************
//Function:
std::vector<glm::vec3> _GameObject::getTriangle()
{
	return m_pModel->getTriangle();
}
