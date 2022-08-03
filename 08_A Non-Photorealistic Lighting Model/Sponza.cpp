#include "Sponza.h"
#include "Interface.h"

_Sponza::_Sponza(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

void _Sponza::initV()
{
	//setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/Dragon/dragon.obj"));
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/objects/nanosuit/nanosuit.obj"));
	setScale(glm::vec3(0.1));
}

void _Sponza::updateV()
{
}