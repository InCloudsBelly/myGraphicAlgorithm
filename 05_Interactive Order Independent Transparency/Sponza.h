#pragma once
#include "GameObject.h"

class _Sponza : public _GameObject
{
public:
	_Sponza(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~_Sponza();

	virtual void initV() override;
	virtual void updateV() override;
};