#include "CornellBox.h"
#include "Interface.h"

CornellBox::CornellBox(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

void CornellBox::initV()
{
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/lost_empire/lost_empire.obj"));
}

void CornellBox::updateV()
{
}