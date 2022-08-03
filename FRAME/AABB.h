//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include "Bounding.h"
#include "FRAME_EXPORTS.h"

class FRAME_DLLEXPORTS _AABB : public _Bounding
{
public:
	_AABB() = default;
	_AABB(const glm::vec3& vMin, const glm::vec3& vMax);
	_AABB(const std::vector<glm::vec3> vPoints);
	virtual ~_AABB() = default;

	virtual float getVolume() const override;

	glm::vec3 getCentre() const;
	glm::vec3 getHalfSize() const;
	glm::vec3 getDiagonal() const;
	const glm::vec3& getMin() const { return m_Min; }
	const glm::vec3& getMax() const { return m_Max; }
	float getSurfaceArea() const;

	void combine(const _AABB& vAnotherAABB);
	void combine(const glm::vec3& vPoint);
	bool overlaps(const _AABB& vAnotherAABB);
	bool inside(const glm::vec3& vPoint);

private:
	glm::vec3 m_Min;
	glm::vec3 m_Max;
};