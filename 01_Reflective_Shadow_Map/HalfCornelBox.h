#pragma once
#include "GameObject.h"

class _HalfCornellBox : public _GameObject
{
public:
	_HalfCornellBox(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~_HalfCornellBox();

	virtual void initV() override;
	virtual void updateV() override;
};