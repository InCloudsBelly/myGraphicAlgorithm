#pragma once
#include "GameObject.h"

class _Dragon : public _GameObject
{
public:
	_Dragon(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~_Dragon();

	virtual void initV() override;
	virtual void updateV() override;
};