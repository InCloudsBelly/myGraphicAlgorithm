#include "Sponza.h"
#include "Interface.h"
#include "AABB.h"
_Sponza::_Sponza(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

_Sponza::~_Sponza()
{
}

void _Sponza::initV()
{
	//setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/VioletSponza/SponzaPBR.obj"));

	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/lost_empire/lost_empire.obj"));
}

void _Sponza::updateV()
{
}