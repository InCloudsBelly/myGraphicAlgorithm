#include "CustomGUI.h"
#include "Interface.h"

_CustomGUI::_CustomGUI(const std::string& vName, int vExcutionOrder) : _GUI(vName, vExcutionOrder)
{
}

_CustomGUI::~_CustomGUI()
{
}

void _CustomGUI::initV()
{
	MyGraphics::ResourceManager::registerSharedData("LightPos", m_LightPos);
	MyGraphics::ResourceManager::registerSharedData("LightDir", m_LightDir);
	MyGraphics::ResourceManager::registerSharedData("LightIntensity", m_Intensity);
	MyGraphics::ResourceManager::registerSharedData("LightColor", m_LightColor);
}

void _CustomGUI::updateV()
{
}