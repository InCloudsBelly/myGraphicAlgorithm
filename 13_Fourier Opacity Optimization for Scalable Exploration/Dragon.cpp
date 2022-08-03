#include "Dragon.h"
#include "Interface.h"

_Dragon::_Dragon(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

_Dragon::~_Dragon()
{
}

void _Dragon::initV()
{
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/Dragon/dragon.obj"));	
}

void _Dragon::updateV()
{
}