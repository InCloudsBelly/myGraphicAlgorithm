//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "AABB.h"

_AABB::_AABB(const glm::vec3& vMin, const glm::vec3& vMax) : m_Min(vMin), m_Max(vMax)
{
}

_AABB::_AABB(const std::vector<glm::vec3> vPoints)
{
	m_Min = { FLT_MAX,FLT_MAX,FLT_MAX };
	m_Max = { -FLT_MAX,-FLT_MAX,-FLT_MAX };

	size_t PointsNum = vPoints.size();
	for (size_t i = 0; i < PointsNum; ++i)
	{
		combine(vPoints[i]);
	}
}

//************************************************************************************
//Function:
float _AABB::getVolume() const
{
	glm::vec3 Diagonal = m_Max - m_Min;
	return Diagonal.x * Diagonal.y * Diagonal.z;
}

//************************************************************************************
//Function:
float _AABB::getSurfaceArea() const
{
	glm::vec3 Diagonal = m_Max - m_Min;
	return 2.0f * (Diagonal.x * Diagonal.y + Diagonal.y * Diagonal.z + Diagonal.z * Diagonal.x);
}

//************************************************************************************
//Function:
glm::vec3 _AABB::getCentre()const
{
	return (m_Min + m_Max) * 0.5f;
}

//************************************************************************************
//Function:
glm::vec3 _AABB::getHalfSize() const
{
	return (m_Max - m_Min) * 0.5f;
}

//************************************************************************************
//Function:
glm::vec3 _AABB::getDiagonal() const
{
	return m_Max - m_Min;
}

//************************************************************************************
//Function:
void _AABB::combine(const _AABB& vAnotherAABB)
{
	m_Min = glm::min(m_Min, vAnotherAABB.m_Min);
	m_Max = glm::max(m_Max, vAnotherAABB.m_Max);
}

//************************************************************************************
//Function:
void _AABB::combine(const glm::vec3& vPoint)
{
	m_Min = glm::min(m_Min, vPoint);
	m_Max = glm::max(m_Max, vPoint);
}

//************************************************************************************
//Function:
bool _AABB::overlaps(const _AABB& vAnotherAABB)
{
	return   m_Max.x > vAnotherAABB.m_Min.x && m_Min.x < vAnotherAABB.m_Max.x
		&&   m_Max.y > vAnotherAABB.m_Min.y && m_Min.y < vAnotherAABB.m_Max.y
		&&   m_Max.z > vAnotherAABB.m_Min.z && m_Min.z < vAnotherAABB.m_Max.z;
}

//************************************************************************************
//Function:
bool _AABB::inside(const glm::vec3& vPoint)
{
	return   m_Max.x > vPoint.x && m_Min.x < vPoint.x
		&&   m_Max.y > vPoint.y && m_Min.y < vPoint.y
		&&   m_Max.z > vPoint.z && m_Min.z < vPoint.z;
}