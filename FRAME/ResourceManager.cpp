//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "ResourceManager.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <iostream>
#include "Camera.h"
#include "UBO4ProjectionWorld.h"
#include "Shader.h"
#include "Utils.h"
#include "Model.h"
#include "RenderPass.h"
#include "GameObject.h"
#include "GUI.h"
#include "MainGUI.h"
#include "GLFWWindow.h"

_ResourceManager::_ResourceManager()
{
}

_ResourceManager::~_ResourceManager()
{
}

//************************************************************************************
//Function:
void _ResourceManager::init()
{
	m_RenderPassSet.reserve(10);
}

//************************************************************************************
//Function: Note:
GLint _ResourceManager::getOrCreateScreenQuadVAO()
{
	if (m_ScreenQuadVAO == 0)      //Fixed Me: some method to simply these code
		m_ScreenQuadVAO = createVAO4ScreenQuad();
	return m_ScreenQuadVAO;
}

//************************************************************************************
//Function:
GLint _ResourceManager::getOrCreateCubeVAO()
{
	if (m_CubeVAO == 0)
		m_CubeVAO = createVAO4Cube();
	return m_CubeVAO;
}

//************************************************************************************
//Function:
GLint _ResourceManager::getOrCretaeSphereVAO()
{
	if (m_SphereVAO == 0)
		m_SphereVAO = createVAO4Sphere();
	return m_SphereVAO;
}

//************************************************************************************
//Function:
const std::shared_ptr<_Model>& _ResourceManager::getOrCreateModel(const std::string &vModelPath)
{
	std::vector<std::string> SplitStringSet;
	boost::split(SplitStringSet, vModelPath, boost::is_any_of("/\\"));
	std::string ModelName = *(SplitStringSet.end() - 1);
	if (m_ModelMap.find(ModelName) == m_ModelMap.end())
		m_ModelMap[ModelName] = std::make_shared<_Model>(vModelPath);
	return m_ModelMap[ModelName];
}

//************************************************************************************
//Function:
const std::shared_ptr<_MainGUI>& _ResourceManager::getOrCreateMainGUI()
{
	if (!m_pMainGUI)
		m_pMainGUI = std::make_shared<_MainGUI>();
	return m_pMainGUI;
}

//************************************************************************************
//Function:
std::shared_ptr<_UBO4ProjectionWorld> _ResourceManager::fetchOrCreateUBO4ProjectionWorld()
{
	if(!m_pUBO4ProjectionWorld)
		m_pUBO4ProjectionWorld= std::make_shared<_UBO4ProjectionWorld>();
	return m_pUBO4ProjectionWorld; 
}

//************************************************************************************
//Function:
std::shared_ptr<_Camera> _ResourceManager::fecthOrCreateMainCamera()
{
	if(!m_pMainCamera)
		m_pMainCamera = std::make_shared<_Camera>();
	return m_pMainCamera; 
}

//************************************************************************************
//Function:
std::shared_ptr<_GLFWWindow> _ResourceManager::fetchOrCreateGLFWWindow()
{
	if (!m_pGLFWWindow)
		m_pGLFWWindow = std::make_shared<_GLFWWindow>();
	return m_pGLFWWindow;
}

//************************************************************************************
//Function:
std::shared_ptr<_Tools>	_ResourceManager::fetchOrCreateTools()
{
	if (!m_pTools)
		m_pTools = std::make_shared<_Tools>();
	return m_pTools;
}

//************************************************************************************
//Function:
void _ResourceManager::registerRenderPass(const std::shared_ptr<_RenderPass> &vRenderPass)
{
	_ASSERT(vRenderPass);
	m_RenderPassSet.push_back(vRenderPass->getPassName(), vRenderPass, 1);
}

//************************************************************************************
//Function:
const std::shared_ptr<_RenderPass>& _ResourceManager::getRenderPassByName(const std::string &vPassName)
{
	_ASSERT(m_RenderPassSet.find(vPassName));
	return m_RenderPassSet[vPassName];
}

//************************************************************************************
//Function:
GLvoid _ResourceManager::registerGameObject(const std::shared_ptr<_GameObject> &vGameObject)
{
	_ASSERT(vGameObject);
	m_GameObjectSet.push_back(vGameObject->getGameObjectName(), vGameObject, 1);
}

//************************************************************************************
//Function:
const std::shared_ptr<_GameObject>& _ResourceManager::getGameObjectByName(const std::string &vGameObjectName)
{
	_ASSERT(m_GameObjectSet.find(vGameObjectName));
	return m_GameObjectSet[vGameObjectName];
}

//************************************************************************************
//Function:
GLvoid _ResourceManager::registerSharedData(const std::string& vDataName, boost::any vData)
{
	_ASSERT(!vDataName.empty());
	_WARNING(m_SharedDataMap.find(vDataName) != m_SharedDataMap.end(), "WARNING: the data called " + vDataName + " has existed in Resource Manager.");
	m_SharedDataMap[vDataName] = vData;
}

//************************************************************************************
//Function:
const boost::any& _ResourceManager::getSharedDataByName(const std::string &vDataName)
{
	_ASSERT(m_SharedDataMap.find(vDataName) != m_SharedDataMap.end());
	return m_SharedDataMap[vDataName];
}

//************************************************************************************
//Function:
boost::any&	_ResourceManager::fetchSharedDataByName(const std::string& vDataName)
{
	_ASSERT(m_SharedDataMap.find(vDataName) != m_SharedDataMap.end());
	return m_SharedDataMap[vDataName];
}

//************************************************************************************
//Function:
GLvoid _ResourceManager::registerSubGUI(const std::shared_ptr<_GUI> &vSubGUI)
{
	_ASSERT(vSubGUI);
	m_SubGUISet.push_back(vSubGUI->getName(), vSubGUI, 1);
}

//************************************************************************************
//Function:
const std::shared_ptr<_GUI>& _ResourceManager::getSubGUIByName(const std::string &vSubGUIByName)
{
	_ASSERT(m_SubGUISet.find(vSubGUIByName));
	return m_SubGUISet[vSubGUIByName];
}

//************************************************************************************
//Function:
void _ResourceManager::updateSharedDataByName(const std::string& vDataName, const boost::any& vData)
{
	//_ASSERT(m_SharedDataMap.find(vDataName) != m_SharedDataMap.end());
	m_SharedDataMap[vDataName] = vData;
}

