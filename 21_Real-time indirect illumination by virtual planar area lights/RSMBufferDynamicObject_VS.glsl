#version 430 core
layout(location = 0) in vec3 _Position;
layout(location = 1) in vec3 _Normal;
//layout(location = 2) in vec2 _TexCoord;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4 u_ModelMatrix;
uniform mat4 u_LightVPMatrix;

//out vec2 v2f_TexCoords;
out vec3 v2f_WorldNormal;
out vec3 v2f_WorldPos;

void main()
{
	vec4 FragPosInWorldSpace = u_ModelMatrix * vec4(_Position, 1.0f);
	gl_Position = u_LightVPMatrix * FragPosInWorldSpace;
	//v2f_TexCoords = _TexCoord;
	//�洢����������ռ��µ�λ���Լ����ߣ����ǹ�Դ�ռ��µ�
	v2f_WorldNormal = normalize(mat3(transpose(inverse(u_ModelMatrix))) * _Normal);	//�����������������˴�����
	v2f_WorldPos = vec3(FragPosInWorldSpace);
}