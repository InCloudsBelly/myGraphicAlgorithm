//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include "random"

class _Tools
{
public:
	_Tools();
	~_Tools();
	
public:
	float getRandom01();
	int   getRandomIntRange(int min, int max);

private:
	std::default_random_engine e;
};
