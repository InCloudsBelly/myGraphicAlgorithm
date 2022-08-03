//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "Interface.h"
#include "App.h"
#include "Common.h"
#include "Camera.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "UBO4ProjectionWorld.h"

//************************************************************************************
//Function:
void MyGraphics::App::initApp()
{
	_App::getOrCreateInstance()->init();
}

//************************************************************************************
//Function:
void MyGraphics::App::updateApp()
{
	_App::getOrCreateInstance()->update();
}

//************************************************************************************
//Function:
double MyGraphics::App::getDeltaTime()
{
	return _App::getOrCreateInstance()->getDeltaTime();
}

//************************************************************************************
//Function:
double MyGraphics::App::getCurrentTime()
{
	return _App::getOrCreateInstance()->getCurrentTime();
}

//************************************************************************************
//Function:
double MyGraphics::App::getFrameRateInMilliSecond()
{
	return _App::getOrCreateInstance()->getFrameRateInMilliSecond();
}

//************************************************************************************
//Function:
int MyGraphics::App::getFramesPerSecond()
{
	return _App::getOrCreateInstance()->getFramesPerSecond();
}

//************************************************************************************
//Function:
int MyGraphics::WINDOW_KEYWORD::getWindowWidth()
{
	return WINDOW_WIDTH;
}

//************************************************************************************
//Function:
int MyGraphics::WINDOW_KEYWORD::getWindowHeight()
{
	return WINDOW_HEIGHT;
}

//************************************************************************************
//Function:
void MyGraphics::WINDOW_KEYWORD::setWindowSize(int vWidth, int vHeight, bool vIsViewportSizeChangedWithWindow/* = true*/)
{
	WINDOW_WIDTH = vWidth;
	WINDOW_HEIGHT = vHeight;
	if (vIsViewportSizeChangedWithWindow)
	{
		VIEWPORT_WIDTH = vWidth;
		VIEWPORT_HEIGHT = vHeight;
	}
}

//************************************************************************************
//Function:
void MyGraphics::WINDOW_KEYWORD::setViewportSize(int vWidth, int vHeight)
{
	VIEWPORT_WIDTH = vWidth;
	VIEWPORT_HEIGHT = vHeight;
}

//************************************************************************************
//Function:
void MyGraphics::WINDOW_KEYWORD::setSampleNum(int vSampleNum)
{
	NUM_SAMPLES = vSampleNum;
}

//************************************************************************************
//Function:
void MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(bool vIsCursorDisable)
{
	CURSOR_DISABLE = vIsCursorDisable;
}

//************************************************************************************
//Function:
void MyGraphics::WINDOW_KEYWORD::setWindowTile(const std::string& vWindowTitle)
{
	WINDOW_TITLE = vWindowTitle;
}

//************************************************************************************
//Function:
void MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(bool vIsEnableGUI)
{
	IS_ENABLE_GUI = vIsEnableGUI;
}

//************************************************************************************
//Function:
const glm::dvec3& MyGraphics::Camera::getMainCameraPos()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraPos();
}

//************************************************************************************
//Function:
glm::dvec3 MyGraphics::Camera::getMainCameraLookAtPos()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getLookAtPos();
}
glm::dvec3 MyGraphics::Camera::getMainCameraDir()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraFront();
}

//************************************************************************************
//Function:
const glm::dvec3& MyGraphics::Camera::getMainCameraUp()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraUp();
}

//************************************************************************************
//Function:
double MyGraphics::Camera::getMainCameraFov()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraFov();
}

//************************************************************************************
//Function:
double MyGraphics::Camera::getMainCameraNear()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraNear();
}

//************************************************************************************
//Function:
double MyGraphics::Camera::getMainCameraFar()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getCameraFar();
}

//************************************************************************************
//Function:
glm::mat4 MyGraphics::Camera::getMainCameraProjectionMatrix()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getProjectionMatrix();
}

//************************************************************************************
//Function:
glm::mat4 MyGraphics::Camera::getMainCameraViewMatrix()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getViewMatrix();
}

//************************************************************************************
//Function:
glm::mat4 MyGraphics::Camera::getMainCameraPrevViewMatrix()
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->getPrevViewMatrix();
}

//************************************************************************************
//Function:
void MyGraphics::Camera::setMainCameraPos(glm::dvec3 vCameraPos)
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setCameraPos(vCameraPos);
}

//************************************************************************************
//Function:
void MyGraphics::Camera::setMainCameraFront(glm::dvec3 vCameraFront)
{
	return _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setCameraFront(vCameraFront);
}

//************************************************************************************
//Function:
void MyGraphics::Camera::setMainCameraFarPlane(double vFarPlane)
{
	auto pMainCamera = _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera();
	pMainCamera->setFarPlane(vFarPlane);
	_ResourceManager::getOrCreateInstance()->fetchOrCreateUBO4ProjectionWorld()->updateProjectionMatrix(pMainCamera->getProjectionMatrix());
}

//************************************************************************************
//Function:
void MyGraphics::Camera::setMainCameraMoveSpeed(double vMoveSpeed)
{
	_ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setMoveSpeed(vMoveSpeed);
}

//************************************************************************************
//Function:
void MyGraphics::Camera::setMainCameraFov(double vFov)
{
	auto pMainCamera = _ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera();
	pMainCamera->setFov(vFov);
	_ResourceManager::getOrCreateInstance()->fetchOrCreateUBO4ProjectionWorld()->updateProjectionMatrix(pMainCamera->getProjectionMatrix());
}

//************************************************************************************
//Function:
void MyGraphics::Camera::setIsEnableCursor(bool vIsEnableCursor)
{
	_ResourceManager::getOrCreateInstance()->fecthOrCreateMainCamera()->setEnableCursor(vIsEnableCursor);
}

//************************************************************************************
//Function:
const std::shared_ptr<_GameObject>& MyGraphics::ResourceManager::getGameObjectByName(const std::string &vGameObjectName)
{
	return _ResourceManager::getOrCreateInstance()->getGameObjectByName(vGameObjectName);
}

//************************************************************************************
//Function:
void MyGraphics::ResourceManager::registerSharedData(const std::string& vDataName, boost::any vData)
{
	_ResourceManager::getOrCreateInstance()->registerSharedData(vDataName, vData);
}

//************************************************************************************
//Function:
void MyGraphics::ResourceManager::registerRenderPass(const std::shared_ptr<_RenderPass> &vRenderPass)
{
	_ResourceManager::getOrCreateInstance()->registerRenderPass(vRenderPass);
}

//************************************************************************************
//Function:
void MyGraphics::ResourceManager::registerGameObject(const std::shared_ptr<_GameObject> &vGameObject)
{
	_ResourceManager::getOrCreateInstance()->registerGameObject(vGameObject);
}

//************************************************************************************
//Function:
void MyGraphics::ResourceManager::registerSubGUI(const std::shared_ptr<_GUI> &vSubGUI)
{
	_ResourceManager::getOrCreateInstance()->registerSubGUI(vSubGUI);
}

//************************************************************************************
//Function:
const std::shared_ptr<_Model>& MyGraphics::ResourceManager::getOrCreateModel(const std::string &vModelPath) 
{
	return _ResourceManager::getOrCreateInstance()->getOrCreateModel(vModelPath);
}

//************************************************************************************
//Function:
GLint MyGraphics::ResourceManager::getOrCreateScreenQuadVAO()
{
	return _ResourceManager::getOrCreateInstance()->getOrCreateScreenQuadVAO();
}

//************************************************************************************
//Function:
GLint MyGraphics::ResourceManager::getOrCreateCubeVAO()
{
	return _ResourceManager::getOrCreateInstance()->getOrCreateCubeVAO();
}

//************************************************************************************
//Function:
const std::shared_ptr<_MainGUI>& MyGraphics::ResourceManager::getOrCreateMainGUI()
{
	return _ResourceManager::getOrCreateInstance()->getOrCreateMainGUI();
}

//************************************************************************************
//Function:
const boost::any& MyGraphics::ResourceManager::getSharedDataByName(const std::string &vDataName)
{
	return _ResourceManager::getOrCreateInstance()->getSharedDataByName(vDataName);
}

//************************************************************************************
//Function:
void MyGraphics::ResourceManager::updateSharedDataByName(const std::string& vDataName, const boost::any& vData)
{
	_ResourceManager::getOrCreateInstance()->updateSharedDataByName(vDataName, vData);
}

//************************************************************************************
//Function:
int MyGraphics::InputManager::getKeyStatus(int vKey)
{
	return _InputManager::getOrCreateInstance()->getKeyStatus()[vKey];
}

//************************************************************************************
//Function:
float MyGraphics::Tools::getRandom01()
{
	return _ResourceManager::getOrCreateInstance()->fetchOrCreateTools()->getRandom01();
}

//************************************************************************************
//Function:
int MyGraphics::Tools::getRandomIntRange(int min, int max)
{
	return _ResourceManager::getOrCreateInstance()->fetchOrCreateTools()->getRandomIntRange(min,max);
}