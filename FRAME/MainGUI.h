//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include "FRAME_EXPORTS.h"
#include <memory>
#include <map>
#include <vector>

struct _mGuiIO;
struct _mFont;

class _MainGUI
{
public:
	void init();
	void update();
	void lateUpdate();

	_mGuiIO* getIO() const { return m_pIO; }

private:
	_mGuiIO*					m_pIO = nullptr;
	std::shared_ptr<_mFont>		m_pDefaultFont;
	std::vector<unsigned short>	m_IconRanges = { 0xf000,0xf3ff,0 };
	bool m_IsShowDefaultWindow = true;
	bool m_IsEndDefaultWindowInCurrentFrame = false;
};