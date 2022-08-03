#include "Ground.h"
#include "Interface.h"

_Ground::_Ground(const std::string& vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

void _Ground::initV()
{
	setVAO(MyGraphics::ResourceManager::getOrCreateScreenQuadVAO());
	rotateX(90);
	//scale(glm::vec3(550, 550, 1));
	scale(glm::vec3(10, 10, 1));
}

void _Ground::updateV()
{
}