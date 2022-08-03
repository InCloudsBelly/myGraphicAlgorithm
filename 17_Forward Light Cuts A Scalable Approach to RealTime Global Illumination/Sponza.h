#pragma once
#include "GameObject.h"

class _Sponza : public _GameObject
{
public:
	_Sponza(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~_Sponza();

	virtual void initV() override;
	virtual void updateV() override;
	std::vector<glm::vec3> getMTriangle() {
		return m_Triangle;
	}
private:
	std::vector<glm::vec3> m_Triangle;
};