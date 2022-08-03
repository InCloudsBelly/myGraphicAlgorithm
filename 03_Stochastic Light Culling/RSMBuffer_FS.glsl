#version 430 core

in  vec3 v2f_FragPos;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;

layout (location = 0) out vec4 RandianceIntensity_;;
layout (location = 1) out vec4 Normal_;
layout (location = 2) out vec4 Position_;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform sampler2D u_DiffuseTexture;
uniform vec3  u_LightColor = vec3(1);
uniform vec3  u_LightDir;
uniform int   u_RSMSize;
uniform int   u_VPLsCount;
uniform float u_RSMCameraAreaInWorldSpace;

void main()
{
	vec3 Normal = normalize(v2f_Normal);
	Normal_ = vec4(Normal, 1.0f);

	vec3 TexelColor = textureLod(u_DiffuseTexture, v2f_TexCoords, 10).rgb;
	TexelColor = pow(TexelColor, vec3(2.2f));
	vec3 LightDir = vec3(u_LightDir);
	vec3 RandianceIntensity = u_LightColor * TexelColor / u_VPLsCount * u_RSMCameraAreaInWorldSpace;/** max(dot(Normal, -LightDirInViewSpace), 0)*/	//�Ȱ�VPLs�������ˣ��������ߵ�˵������Ӧ�ó��������ֵ
	RandianceIntensity_ = vec4(RandianceIntensity, 1.0f);
	Position_ = vec4(v2f_FragPos, 1.0f);
}