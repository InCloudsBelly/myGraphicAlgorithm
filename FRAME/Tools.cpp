//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "Tools.h"
#include <iostream>
#include "InputManager.h"
#include "App.h"
#include "common.h"

_Tools::_Tools()
{

}

_Tools::~_Tools()
{
}

//************************************************************************************
//Function:
float _Tools::getRandom01()
{
	std::uniform_real_distribution<float> u(0, 1);
	return u(e);
}

//************************************************************************************
//Function:
int  _Tools::getRandomIntRange(int min,int max)
{
	std::uniform_int_distribution<int> u(min, max);
	return u(e);
}
