#include "Sponza.h"
#include "Interface.h"

_Sponza::_Sponza(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

_Sponza::~_Sponza()
{
}

void _Sponza::initV()
{
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/sponza/sponza.obj"));	//../Model/CornellBox/HalfCornell/HalfCornell.obj
	//auto AABB = getAABB();
	//setScale({ 300,300,300 });
}

void _Sponza::updateV()
{
}