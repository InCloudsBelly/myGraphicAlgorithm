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
	MyGraphics::ResourceManager::registerSharedData("NormalThreshold", m_NormalThresholdInEdgeDetecion);
	MyGraphics::ResourceManager::registerSharedData("DepthThreshold", m_DepthThresholdInEdgeDetection);
}

void _CustomGUI::updateV()
{
#ifdef _DEBUG
	glm::vec3 MinLightPos = glm::vec3(-1000), MaxLightPos = glm::vec3(1000);
	float LightPosDragSpeed = 0.01f;
	if (_GUI::dragScalarN("LightPos", MyGraphics::EDataType::DataType_Float, &m_LightPos, 3, LightPosDragSpeed, &MinLightPos, &MaxLightPos))
		MyGraphics::ResourceManager::updateSharedDataByName("LightPos", m_LightPos);
	glm::vec3 MinLightDir = glm::vec3(-1, -1, -1), MaxLightDir = glm::vec3(1, 1, 1);
	float LightDirDragSpeed = 0.01f;
	if (_GUI::dragScalarN("LightDir", MyGraphics::EDataType::DataType_Float, &m_LightDir, 3, LightDirDragSpeed, &MinLightDir, &MaxLightDir))
		MyGraphics::ResourceManager::updateSharedDataByName("LightDir", normalize(m_LightDir));
#endif
}