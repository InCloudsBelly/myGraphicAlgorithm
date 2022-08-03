#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>

class _Sponza : public _GameObject
{
public:
	_Sponza(const std::string& vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;
};