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
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/Lucy/Lucy.obj"));//../Model/Dragon/dragon.obj
	setScale({ 0.005,0.005,0.005 });
	rotateY(180);
	translate({ -0.2, -2.2, -0.56});
}

void _DynamicObject::updateV()
{
	//moveLoop({ 0.0f, 1.0f, 1.2f }, 8.0f);
	//rotateLoop({ 0.0f, 2.0f, 0.0f });
	//setIsFirstFrame(false);
}