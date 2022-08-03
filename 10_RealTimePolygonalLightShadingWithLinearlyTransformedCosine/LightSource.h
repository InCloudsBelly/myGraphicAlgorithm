#pragma once
#include "GameObject.h"
#include <vector>

class _LightSource : public _GameObject
{
public:
	_LightSource(const std::string &vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;

	const std::vector<glm::vec3>& getPolygonalLightVertexPosSet() const { return m_PolygonalLightVertexPosSet; }

private:
	std::vector<glm::vec3> m_PolygonalLightVertexOriginalPosSet;
	std::vector<glm::vec3> m_PolygonalLightVertexPosSet;
	glm::vec3 m_Rotation;
};