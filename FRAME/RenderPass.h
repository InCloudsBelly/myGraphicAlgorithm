//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <string>
#include <memory>
#include "FRAME_EXPORTS.h"
#include "Common.h"
class _Shader;

class FRAME_DLLEXPORTS _RenderPass
{
public:
	_RenderPass();
	_RenderPass(const std::string& vPassName, int vExcutionOrder);
	_RenderPass(const std::string& vPassName, int vExcutionOrder,MyGraphics::ERenderPassType vtype);
	virtual ~_RenderPass();

	virtual void initV() = 0;
	virtual void updateV() = 0;

	bool operator<(const _RenderPass& vOtherPass) const;

	const std::string& getPassName() const { return m_PassName; }
	int getExecutionOrder() const { return m_ExecutionOrder; }

	void setPassName(const std::string &vPassName) { m_PassName = vPassName; }
	void setExecutionOrder(int vExecutionOrder) { m_ExecutionOrder = vExecutionOrder; }
	MyGraphics::ERenderPassType getPassType();
	void setPassType(MyGraphics::ERenderPassType vType);
	int getExecutionOrder();
	void finishExecute();
protected:
	std::shared_ptr<_Shader> m_pShader;

private:
	std::string m_PassName;
	MyGraphics::ERenderPassType m_Type  = MyGraphics::ERenderPassType::RenderPassType_Normal;
	int m_ExecutionOrder = -1;
};