//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "App.h"
#include <crtdbg.h>
#include "Utils.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Camera.h"
#include "UBO4ProjectionWorld.h"
#include "RenderPass.h"
#include "GLFWWindow.h"
#include "InputManager.h"
#include "GUI.h"
#include "MainGUI.h"
#include "GameObject.h"

_App::_App()
{
}

_App::~_App()
{
}

//************************************************************************************
//Function:
GLvoid _App::init()
{
	_ResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->init();
	m_pWindow = _ResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->fetchWindow();
	m_pResourceManager = _ResourceManager::getOrCreateInstance();
	m_pResourceManager->init();
	m_pResourceManager->getOrCreateMainGUI()->init();
	m_pResourceManager->fecthOrCreateMainCamera()->init();
	_InputManager::getOrCreateInstance()->init();
	m_pResourceManager->fetchOrCreateUBO4ProjectionWorld()->init();

	for (auto &vItem : m_pResourceManager->getSubGUISet())
	{
		vItem->initV();
	}

	for (auto &vItem : m_pResourceManager->getGameObjectSet())
	{
		vItem->initV();
	}

	for (auto &vItem : m_pResourceManager->getRenderPassSet())
	{
		vItem->initV();
	}
}

//************************************************************************************
//Function:
GLvoid _App::update()
{
	_ASSERT(m_pWindow);
	while (!glfwWindowShouldClose(m_pWindow))
	{
		__calculateTime();
		glfwPollEvents();
		m_pResourceManager->fecthOrCreateMainCamera()->update();
		m_pResourceManager->fetchOrCreateUBO4ProjectionWorld()->update();

		for (auto &vItem : m_pResourceManager->getGameObjectSet())
		{
			vItem->updateV();
		}

		glClear(GL_COLOR_BUFFER_BIT);

		MyGraphics::_Vector<std::shared_ptr<_RenderPass>> renderPassSets = m_pResourceManager->getRenderPassSet();
		for (int i = 0; i < renderPassSets.size(); i++)
		{
			if (renderPassSets[i]->getExecutionOrder() == -1) continue;
			switch (renderPassSets[i]->getPassType())
			{
			case MyGraphics::ERenderPassType::RenderPassType_Once:
			{
				renderPassSets[i]->updateV();
				renderPassSets[i]->finishExecute();
				break;
			}
			case MyGraphics::ERenderPassType::RenderPassType_Parallel:
			{
				int j = i;
				while (j < renderPassSets.size())
				{
					if (renderPassSets[i]->getExecutionOrder() != renderPassSets[j]->getExecutionOrder())break;
					renderPassSets[j++]->updateV();
				}
				i = j - 1;
				break;
			}
			case MyGraphics::ERenderPassType::RenderPassType_ParallelOnce:
			{
				int j = i;
				while (j < renderPassSets.size())
				{
					if (renderPassSets[i]->getExecutionOrder() != renderPassSets[j]->getExecutionOrder())break;

					renderPassSets[j++]->updateV();
				}
				while (i < j)
				{
					renderPassSets[i++]->finishExecute();
				}
				i--;
				break;

			}
			case MyGraphics::ERenderPassType::RenderPassType_Delay:
				renderPassSets[i]->setPassType(MyGraphics::ERenderPassType::RenderPassType_Normal);
				break;
			default:
				renderPassSets[i]->updateV();
				break;
			}
		}

		if (MyGraphics::COMPONENT_CONFIG::IS_ENABLE_GUI)
		{
			m_pResourceManager->getOrCreateMainGUI()->update();
			for (auto &vItem : m_pResourceManager->getSubGUISet())
			{
				vItem->updateV();
			}
			m_pResourceManager->getOrCreateMainGUI()->lateUpdate();
		}

		glfwSwapBuffers(m_pWindow);
	}
}

//************************************************************************************
//Function:
GLvoid _App::__calculateTime()
{
	m_CurrentTime = glfwGetTime();
	m_DeltaTime = m_CurrentTime - m_LastFrameTime;
	m_LastFrameTime = m_CurrentTime;

	++m_FrameCounter;
	m_TimeCounter += m_DeltaTime;
	if (m_TimeCounter >= 1.0)
	{
		m_TimeCounter = 0.0;
		m_FramesPerSecond = m_FrameCounter;
		m_FrameCounter = 0;
	}
}

//************************************************************************************
//Function:
GLdouble _App::getDeltaTime() const
{
	return m_DeltaTime;
}

//************************************************************************************
//Function:
GLdouble _App::getFrameRateInMilliSecond() const
{
	return m_DeltaTime * 1000;
}

//************************************************************************************
//Function:
GLdouble _App::getCurrentTime() const
{
	return m_CurrentTime;
}

//************************************************************************************
//Function:
GLuint _App::getFramesPerSecond() const
{
	return m_FramesPerSecond;
}