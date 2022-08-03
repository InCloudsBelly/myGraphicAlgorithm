#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

struct SPlane
{
	glm::vec4 P0;
	glm::vec4 P1;
	glm::vec4 P2;
	glm::vec4 P3;
	glm::vec4 Normal;
	glm::vec4 SizeAndUV;
	glm::ivec4 Index;//��һλΪ���ڵ�������Ϊ-1����û�и��ڵ㣬�ڶ�λΪ�Һ�������������λΪ�Ϻ�����Ӱ��-1��û�нڵ�
};

class CCalculatePlane : public _RenderPass
{
public:
	CCalculatePlane(const std::string& vPassName, int vExcutionOrder);
	virtual ~CCalculatePlane();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	int ssbo = -1;
};