//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <memory>
#include "GLFWWindow.h"
#include "Singleton.h"

class _Shader;
class _Camera;
class _ResourceManager;

class _App : public _Singleton<_App>
{
	friend class _Singleton<_App>;
public:
	~_App();

	GLvoid init();
	GLvoid update();
	GLdouble getDeltaTime() const;
	GLdouble getFrameRateInMilliSecond() const;
	GLdouble getCurrentTime() const;
	GLuint	 getFramesPerSecond() const;

private:
	_App();
	GLvoid __calculateTime();

	GLFWwindow  *m_pWindow;
	GLdouble     m_DeltaTime = 0.0;
	GLdouble     m_LastFrameTime = 0.0;
	GLdouble     m_CurrentTime = 0.0;
	GLdouble	 m_TimeCounter = 0.0;
	GLuint		 m_FramesPerSecond = 0;
	GLuint		 m_FrameCounter = 0;
	std::shared_ptr<_ResourceManager> m_pResourceManager = nullptr;
};