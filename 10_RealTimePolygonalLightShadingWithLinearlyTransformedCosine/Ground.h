#pragma once
#include "GameObject.h"

class _Ground : public _GameObject
{
public:
	_Ground(const std::string& vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;
};