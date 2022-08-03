//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "Common.h"

using namespace MyGraphics;

int MyGraphics::WINDOW_KEYWORD::WINDOW_WIDTH = 800;
int MyGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT = 600;
int MyGraphics::WINDOW_KEYWORD::VIEWPORT_WIDTH = MyGraphics::WINDOW_KEYWORD::WINDOW_WIDTH;
int MyGraphics::WINDOW_KEYWORD::VIEWPORT_HEIGHT = MyGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT;
int MyGraphics::WINDOW_KEYWORD::VIEWPORT_LEFTBOTTOM_X = 0;
int MyGraphics::WINDOW_KEYWORD::VIEWPORT_LEFTBOTTOM_Y = 0;
int MyGraphics::WINDOW_KEYWORD::NUM_SAMPLES = 4;
bool MyGraphics::WINDOW_KEYWORD::CURSOR_DISABLE = true;
std::string MyGraphics::WINDOW_KEYWORD::WINDOW_TITLE = "Graphics";

bool MyGraphics::COMPONENT_CONFIG::IS_ENABLE_GUI = true;