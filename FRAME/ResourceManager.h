//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
//The resource manager is mainly used to store the global resources that will be shared in multiple pass
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <GL/glew.h>
#include <set>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <boost/any.hpp>
#include <iostream>
#include "common.h"
#include "Singleton.h"
#include "FRAME_EXPORTS.h"
#include "MVector.h"
#include "Tools.h"
class _Camera;
class _UBO4ProjectionWorld;
class _Shader;
class _FirstPass;
class _Model;
class _ScreenQuadPass;
class CLoadModelPass;
class _LoadModelCullFrontPass;
class _RenderPass;
class _GameObject;
class _GUI;
class _MainGUI;
class _GLFWWindow;

class _ResourceManager : public _Singleton<_ResourceManager>
{
	friend class _Singleton<_ResourceManager>;
public:
	~_ResourceManager();

	GLvoid  init();
	GLint														getOrCreateScreenQuadVAO();
	GLint														getOrCreateCubeVAO();
	GLint														getOrCretaeSphereVAO();
	const std::shared_ptr<_Model>&								getOrCreateModel(const std::string &vModelPath);
	const std::shared_ptr<_MainGUI>&							getOrCreateMainGUI();
	const std::shared_ptr<_RenderPass>&							getRenderPassByName(const std::string &vPassName);			//Time consuming much
	const std::shared_ptr<_GameObject>&							getGameObjectByName(const std::string &vGameObjectName);	//Time consuming much
	const std::shared_ptr<_GUI>&								getSubGUIByName(const std::string &vSubGUIByName);			//Time consuming much
	const MyGraphics::_Vector<std::shared_ptr<_RenderPass>>&  getRenderPassSet() const { return m_RenderPassSet; }
	const MyGraphics::_Vector<std::shared_ptr<_GameObject>>&  getGameObjectSet() const { return m_GameObjectSet; }
	const MyGraphics::_Vector<std::shared_ptr<_GUI>>&			getSubGUISet() const { return m_SubGUISet; }
	const boost::any&											getSharedDataByName(const std::string &vDataName);
	boost::any&													fetchSharedDataByName(const std::string& vDataName);

	std::shared_ptr<_Camera>              fecthOrCreateMainCamera();
	std::shared_ptr<_UBO4ProjectionWorld> fetchOrCreateUBO4ProjectionWorld();
	std::shared_ptr<_GLFWWindow>		  fetchOrCreateGLFWWindow();
	std::shared_ptr<_Tools>				  fetchOrCreateTools();

	void updateSharedDataByName(const std::string& vDataName, const boost::any& vData);

	GLvoid registerRenderPass(const std::shared_ptr<_RenderPass> &vRenderPass);
	GLvoid registerGameObject(const std::shared_ptr<_GameObject> &vGameObject);
	GLvoid registerSubGUI(const std::shared_ptr<_GUI> &vSubGUI);
	GLvoid registerSharedData(const std::string& vDataName, boost::any vData);

private:
	_ResourceManager();

	std::shared_ptr<_GLFWWindow>			m_pGLFWWindow;
	std::shared_ptr<_UBO4ProjectionWorld>   m_pUBO4ProjectionWorld;
	std::shared_ptr<_Camera>                m_pMainCamera;
	std::shared_ptr<_MainGUI>				m_pMainGUI;
	std::shared_ptr<_Tools>				    m_pTools;

	GLint m_ScreenQuadVAO = 0;
	GLint m_CubeVAO = 0;
	GLint m_SphereVAO = 0;
	GLint m_Texture4FisrtPass = 0;
	GLint m_Texture4LoadModelCullFrontPass = 0;
	MyGraphics::_Vector<std::shared_ptr<_RenderPass>> m_RenderPassSet;
	MyGraphics::_Vector<std::shared_ptr<_GameObject>> m_GameObjectSet;
	MyGraphics::_Vector<std::shared_ptr<_GUI>>		m_SubGUISet;
	std::map<std::string, std::shared_ptr<_Model>> m_ModelMap;
	std::map<std::string, boost::any> m_SharedDataMap;
};