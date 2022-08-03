//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

class _UBO4ProjectionWorld
{
public:
	_UBO4ProjectionWorld() = default;
	void init();
	void update();
	void updateViewMatrix(const glm::mat4& vViewMatrix);
	void updateProjectionMatrix(const glm::mat4& vProjectionMatrix);

private:
	GLint m_UBO = -1;
};