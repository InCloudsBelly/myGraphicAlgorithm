#version 430 core
#pragma optionNV (unroll all)	//��ʱ��֪����û��������

#define LOCAL_GROUP_SIZE 16
#define PI 3.1415926

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

struct SVPL
{
	vec4 PositionInViewSpace;
	vec3 NormalInViewSpaceAndAlphai;
	vec3 RadianceIntensity;
	float Alphai;
	float Radius;
};


layout (std430, binding = 0) buffer VPLs
{
	SVPL u_VPLs[];
};

layout (std430, binding = 1) buffer RandomNumbers
{
	float u_RandomNumbers[];
};

uniform int	  u_RSMSize;
uniform int   u_VPLsCount;
uniform float u_MaxErrorBound;
uniform sampler2D u_RSMRandianceIntensityTexture;
uniform sampler2D u_RSMNormalTexture;		
uniform sampler2D u_RSMPositionTexture;		

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	int FragIndex = FragPos.y * u_RSMSize + FragPos.x;

	vec3 VPLRandianceIntensity = texelFetch(u_RSMRandianceIntensityTexture, FragPos, 0).xyz;
	vec3 VPLNormalInViewSpace = normalize(texelFetch(u_RSMNormalTexture, FragPos, 0).xyz);
	vec3 VPLPositionInViewSpace = texelFetch(u_RSMPositionTexture, FragPos, 0).xyz;
	float MaxRandiaceIntensity = max(max(VPLRandianceIntensity.x, VPLRandianceIntensity.y), VPLRandianceIntensity.z);
	float Alphai = 2 * PI * 0.0005f / MaxRandiaceIntensity;
	float VPLRadius = 1.0f / sqrt(Alphai * u_RandomNumbers[FragIndex]);

	u_VPLs[FragIndex].PositionInViewSpace = vec4(VPLPositionInViewSpace, 1.0f);
	u_VPLs[FragIndex].NormalInViewSpaceAndAlphai = VPLNormalInViewSpace;
	u_VPLs[FragIndex].RadianceIntensity = VPLRandianceIntensity;
	u_VPLs[FragIndex].Alphai = Alphai;
	u_VPLs[FragIndex].Radius = VPLRadius;
}