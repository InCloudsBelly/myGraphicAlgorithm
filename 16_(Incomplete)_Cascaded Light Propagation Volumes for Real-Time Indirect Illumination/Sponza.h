#pragma once
#include "GameObject.h"

class _Sponza : public _GameObject
{
public:
	_Sponza(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~_Sponza();

	virtual void initV() override;
	virtual void updateV() override;
	const glm::vec3& getMin() const { return m_Min; }
	const glm::vec3& getMax() const { return m_Max; }
private:
	std::shared_ptr<_AABB>m_AABB;
	glm::vec3 m_Min;
	glm::vec3 m_Max;
};