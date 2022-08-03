//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "MainGUI.h"
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include "GLFWWindow.h"
#include "App.h"
#include "ResourceManager.h"
//using namespace MyGraphics;

//************************************************************************************
//Function:
void _MainGUI::init()
{
	ImGui::CreateContext();
	m_pIO = &ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(_ResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->fetchWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 430");
	ImGui::StyleColorsDark();

	ImFontConfig IconsConfig;
	IconsConfig.MergeMode = true;
	m_pDefaultFont.reset(m_pIO->Fonts->AddFontFromFileTTF("../Fonts/roboto-regular.ttf", 18.0));
	_ASSERT(m_pDefaultFont);
	m_pIO->Fonts->AddFontFromFileTTF("../Fonts/fontawesome-webfont.ttf", 16.0f, &IconsConfig, m_IconRanges.data());
}

//************************************************************************************
//Function:
void _MainGUI::update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetColorEditOptions(ImGuiColorEditFlags_PickerHueWheel);

	if (m_IsShowDefaultWindow)
	{
		ImGuiWindowFlags WindowFlag = 0;
		WindowFlag |= ImGuiWindowFlags_MenuBar;
		ImGui::Begin("Demo Information", &m_IsShowDefaultWindow, WindowFlag);
		ImGui::Text("Frame rate: %f, FPS: %d  %n CameraPos:", _App::getOrCreateInstance()->getFrameRateInMilliSecond(), _App::getOrCreateInstance()->getFramesPerSecond());
		m_IsEndDefaultWindowInCurrentFrame = false;
	}
}

//************************************************************************************
//Function:
void _MainGUI::lateUpdate()
{
	if (m_IsShowDefaultWindow || !m_IsEndDefaultWindowInCurrentFrame)
	{
		ImGui::End();
		m_IsEndDefaultWindowInCurrentFrame = true;
	}

	ImGui::Render();
	auto p = ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}