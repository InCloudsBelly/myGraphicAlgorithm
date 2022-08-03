#include "LightSource.h"
#include "Interface.h"

_LightSource::_LightSource(const std::string &vGameObjectName, int vExecutionOrder) : _GameObject(vGameObjectName, vExecutionOrder)
{
}

//************************************************************************************
//Function:
void _LightSource::initV()
{
	setVAO(MyGraphics::ResourceManager::getOrCreateScreenQuadVAO());
	translate(glm::vec3(0, 1 , 0));
	m_PolygonalLightVertexOriginalPosSet =
	{
		glm::vec3(1, 1, 0),
		glm::vec3(1, -1, 0),
		glm::vec3(-1, -1, 0),
		glm::vec3(-1, 1, 0)
	};
	m_PolygonalLightVertexPosSet.resize(m_PolygonalLightVertexOriginalPosSet.size());
	for (int i = 0; i < m_PolygonalLightVertexPosSet.size(); ++i)
	{
		m_PolygonalLightVertexPosSet[i] = glm::vec3(getModelMatrix() * glm::vec4(m_PolygonalLightVertexOriginalPosSet[i], 1.0));
	}
}

//************************************************************************************
//Function:
void _LightSource::updateV()
{

}