#include "MyGUI.h"
#include "Interface.h"
#include "Common.h"

_MyGUI::_MyGUI(const std::string& vName, int vExcutionOrder) : _GUI(vName, vExcutionOrder)
{
}

_MyGUI::~_MyGUI()
{
}

//************************************************************************************
//Function:
void _MyGUI::initV()
{
	MyGraphics::ResourceManager::registerSharedData("DiffuseColor", m_DiffuseColor);
	MyGraphics::ResourceManager::registerSharedData("SpecularColor", m_SpecularColor);
	MyGraphics::ResourceManager::registerSharedData("Roughness", m_Roughness);
	MyGraphics::ResourceManager::registerSharedData("Intensity", m_Intensity);
	MyGraphics::ResourceManager::registerSharedData("IsTwoSide", m_IsTwoSide);
	MyGraphics::ResourceManager::registerSharedData("Rotation", m_LightSourceRotation);
}

//************************************************************************************
//Function:
void _MyGUI::updateV()
{
	if (_GUI::colorEdit("Diffuse Color", m_DiffuseColor))
		MyGraphics::ResourceManager::updateSharedDataByName("DiffuseColor", m_DiffuseColor);
	if (_GUI::colorEdit("Specular Color", m_SpecularColor))
		MyGraphics::ResourceManager::updateSharedDataByName("SpecularColor", m_SpecularColor);
	float MinRoughness = 0.0f, MaxRoughness = 1.0f;
	if (_GUI::sliderScalar("Roughness", MyGraphics::EDataType::DataType_Float, &m_Roughness, &MinRoughness, &MaxRoughness))
		MyGraphics::ResourceManager::updateSharedDataByName("Roughness", m_Roughness);
	float MinIntensity = 0.0f, MaxIntensity = 10.0f;
	if (_GUI::sliderScalar("Intensity", MyGraphics::EDataType::DataType_Float, &m_Intensity, &MinIntensity, &MaxIntensity))
		MyGraphics::ResourceManager::updateSharedDataByName("Intensity", m_Intensity);
	float MinRotation = 0.0f, MaxRotation = 360.0f;
	if (_GUI::sliderScalar("Rotation X", MyGraphics::EDataType::DataType_Float, &m_LightSourceRotation.x, &MinRotation, &MaxRotation))
		MyGraphics::ResourceManager::updateSharedDataByName("Rotation", m_LightSourceRotation);
	if(_GUI::sliderScalar("Rotation Y",MyGraphics::EDataType::DataType_Float,&m_LightSourceRotation.y,&MinRotation,&MaxRotation))
		MyGraphics::ResourceManager::updateSharedDataByName("Rotation", m_LightSourceRotation);
	if (_GUI::sliderScalar("Rotation Z", MyGraphics::EDataType::DataType_Float, &m_LightSourceRotation.z, &MinRotation, &MaxRotation))
		MyGraphics::ResourceManager::updateSharedDataByName("Rotation", m_LightSourceRotation);

	if (_GUI::checkBox("IsTwoSide", m_IsTwoSide))
		MyGraphics::ResourceManager::updateSharedDataByName("IsTwoSide", m_IsTwoSide);
}