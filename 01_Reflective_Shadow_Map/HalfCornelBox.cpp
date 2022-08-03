#include "HalfCornelBox.h"
#include "Interface.h"

_HalfCornellBox::_HalfCornellBox(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

_HalfCornellBox::~_HalfCornellBox()
{
}

void _HalfCornellBox::initV()
{
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/Empty/CornellBox.obj"));//../Model/CornellBox/HalfCornell/HalfCornell.obj
	//.. / Model / objects / backpack / backpack.obj
	setPosition(glm::vec3(0, -1, 0));

}

void _HalfCornellBox::updateV()
{
}