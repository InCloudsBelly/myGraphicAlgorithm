#pragma once
#include "GameObject.h"

class Lucy : public _GameObject
{
public:
	Lucy(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~Lucy();

	virtual void initV() override;
	virtual void updateV() override;
};