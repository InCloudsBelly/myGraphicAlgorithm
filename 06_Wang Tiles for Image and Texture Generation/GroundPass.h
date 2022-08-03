#pragma once
#include "RenderPass.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include "Common.h"


class _GroundPass : public _RenderPass
{
public:
	_GroundPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~_GroundPass();

	virtual void initV() override;
	virtual void updateV() override;
private:

};
