#include "Lucy.h"
#include "Interface.h"

Lucy::Lucy(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

Lucy::~Lucy()
{
}

void Lucy::initV()
{
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/objects/vampire/dancing_vampire.dae"));//../Model/CornellBox/HalfCornell/HalfCornell.obj
	setPosition(glm::vec3(-0.3, -1, -0.3));
	setScale(glm::vec3(0.005));
}

void Lucy::updateV()
{
}