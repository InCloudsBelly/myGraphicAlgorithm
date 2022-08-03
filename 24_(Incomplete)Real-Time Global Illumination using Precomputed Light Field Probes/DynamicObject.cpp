#include "DynamicObject.h"
#include "Interface.h"
#include "Utils.h"

_DynamicObject::_DynamicObject(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

_DynamicObject::~_DynamicObject()
{
}

void _DynamicObject::initV()
{
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/teapot/teapot.obj"));
	setScale({ 0.005,0.005,0.005 });
	translate({ -0.2, -1.2, -0.56});
}

void _DynamicObject::updateV()
{
}