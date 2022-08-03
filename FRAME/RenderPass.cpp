//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#include "RenderPass.h"
#include "Shader.h"

_RenderPass::_RenderPass()
{
}

_RenderPass::_RenderPass(const std::string& vPassName, int vExcutionOrder):m_PassName(vPassName),m_ExecutionOrder(vExcutionOrder)
{
}
_RenderPass::_RenderPass(const std::string& vPassName, int vExcutionOrder, MyGraphics::ERenderPassType vtype) : m_PassName(vPassName), m_ExecutionOrder(vExcutionOrder),m_Type(vtype)
{

}
_RenderPass::~_RenderPass()
{
}

//************************************************************************************
//Function:
bool _RenderPass::operator<(const _RenderPass& vOtherPass) const
{
 	return m_ExecutionOrder <= vOtherPass.getExecutionOrder();//保证排序稳定性
}

//************************************************************************************
//Function:
MyGraphics::ERenderPassType _RenderPass::getPassType()
{
	return m_Type;
}
//************************************************************************************
//Function:
void _RenderPass::setPassType(MyGraphics::ERenderPassType vType)
{
	m_Type = vType;
}

//************************************************************************************
//Function:
int _RenderPass::getExecutionOrder()
{
	return m_ExecutionOrder;
}

//************************************************************************************
//Function:
void _RenderPass::finishExecute()
{
	 m_ExecutionOrder = -1;
}