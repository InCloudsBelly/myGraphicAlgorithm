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
	setModel(MyGraphics::ResourceManager::getOrCreateModel("../Model/VioletSponza/SponzaPBR.obj"));
	m_AABB = getAABB();
	m_Min = m_AABB->getMin();
	m_Max = m_AABB->getMax();
	MyGraphics::ResourceManager::registerSharedData("MinAABB", m_Min);
	MyGraphics::ResourceManager::registerSharedData("MaxAABB", m_Max);
}

void _Sponza::updateV()
{
}