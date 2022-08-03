#include "Sponza.h"
#include "Interface.h"

_Sponza::_Sponza(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

void _Sponza::initV()
{
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/sponza/sponza.obj"));
}

void _Sponza::updateV()
{
	
}