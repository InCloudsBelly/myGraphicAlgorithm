//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "GLFWWindow.h"
#include <iostream>
#include "common.h"
//#include "Utils.h"

_GLFWWindow::_GLFWWindow() : m_pWindow(nullptr)
{
}

_GLFWWindow::~_GLFWWindow()
{
}

//************************************************************************************
//Function:
void _GLFWWindow::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//#ifdef MULTISAMPLE
	glfwWindowHint(GLFW_SAMPLES, MyGraphics::WINDOW_KEYWORD::NUM_SAMPLES);
//#endif //MULTISAMPLE
	//m_pWindow = std::make_shared<GLFWwindow>(glfwCreateWindow(MyGraphics::WINDOW_KEYWORD::WINDOW_WIDTH, MyGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT, MyGraphics::WINDOW_KEYWORD::WINDOW_TITLE.c_str(), nullptr, nullptr));
	m_pWindow = glfwCreateWindow(MyGraphics::WINDOW_KEYWORD::WINDOW_WIDTH, MyGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT, MyGraphics::WINDOW_KEYWORD::WINDOW_TITLE.c_str(), nullptr, nullptr);
	if (!m_pWindow)
	{
		std::cerr << "Error::Window:: Window Create Failure" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_pWindow);
	if(MyGraphics::WINDOW_KEYWORD::CURSOR_DISABLE)
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Error::Window:: GLEW Init Failure" << std::endl;
		return;
	}
	__setViewport();
}

//************************************************************************************
//Function:
void _GLFWWindow::__setViewport()
{
	glViewport(MyGraphics::WINDOW_KEYWORD::VIEWPORT_LEFTBOTTOM_X, MyGraphics::WINDOW_KEYWORD::VIEWPORT_LEFTBOTTOM_Y, MyGraphics::WINDOW_KEYWORD::VIEWPORT_WIDTH, MyGraphics::WINDOW_KEYWORD::VIEWPORT_HEIGHT);
}

//************************************************************************************
//Function:
GLFWwindow* _GLFWWindow::fetchWindow() const
{
	return m_pWindow;
}