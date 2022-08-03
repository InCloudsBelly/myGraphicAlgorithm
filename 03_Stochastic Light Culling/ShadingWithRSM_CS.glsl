#version 430 core
#pragma optionNV (unroll all)	//��ʱ��֪����û��������

#define LOCAL_GROUP_SIZE 16
#define PI 3.1415926

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

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

uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_LightDepthTexture;		//��Դ�ռ��µ�NDC���
uniform mat4  u_LightVPMatrix;
uniform int   u_RSMSize;
uniform vec3  u_LightDir;
uniform int   u_VPLsCount;

uniform bool  u_EnableIndirectIllumination;

vec3 calcVPLIrradiance(vec3 vVPLRadianceIntensity, vec3 vVPLNormalAndAlphai, vec3 vVPLPos, vec3 vFragPos, vec3 vFragNormal,float Alphai, float Radius, float vXi4VPL)
{
	vec3 VPL2Frag = vFragPos - vVPLPos;
	float Distance = length(VPL2Frag);
	float SquareDistance = Distance*Distance;
	if(Distance < Radius){
		float Fil = 1.0f / max(SquareDistance,0.1);
		float Pil = min(Fil/Alphai, 1.0f);
		float Atten;
		if(Pil > vXi4VPL)
		{
			Atten = max(Alphai, Fil);
			return vVPLRadianceIntensity  * max(dot(vVPLNormalAndAlphai, VPL2Frag), 0) * max(dot(vFragNormal, -VPL2Frag), 0)  / max(SquareDistance,0.1) * Atten;
		}
	}
	return vec3(0);
}

void main()
{
	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	vec3 FragNormal = texelFetch(u_NormalTexture, uv, 0).xyz;	//��vec3(0)��������һ��
	if((abs(FragNormal.x) < 0.0001f) && (abs(FragNormal.y) < 0.0001f) && (abs(FragNormal.z) < 0.0001f))
	{
		imageStore(u_OutputImage, uv, vec4(0, 0, 0, 1));
		return;
	}
	FragNormal = normalize(FragNormal);
	vec3 FragAlbedo = texelFetch(u_AlbedoTexture, uv, 0).xyz;
	vec3 FragPos = texelFetch(u_PositionTexture, uv, 0).xyz;
	vec4 FragPosInLightSpace = u_LightVPMatrix * vec4(FragPos, 1);
	FragPosInLightSpace /= FragPosInLightSpace.w;
	FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
	vec3 LightDir = u_LightDir;
	vec3 DirectIllumination;
	if(FragPosInLightSpace.z < 0.0f || FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f)
	{
		DirectIllumination = vec3(0.05*FragAlbedo);
	}
	else
	{
		vec2 FragNDCPos4Light = FragPosInLightSpace.xy;
		float ClosetDepth4Light = texture(u_LightDepthTexture, FragNDCPos4Light).r; 
		float Bias = max(0.0001 * (1.0 - dot(FragNormal, -LightDir)), 0.00001);
		float Visibility4DirectLight = (FragPosInLightSpace.z - Bias < ClosetDepth4Light) ? 1.0f : 0.05f;
		DirectIllumination = (Visibility4DirectLight >0.5f)? FragAlbedo * max(dot(-LightDir, FragNormal), 0) * Visibility4DirectLight : 0.05f * FragAlbedo ;
	}
	vec3 IndirectIllumination = vec3(0);
	for(int i = 0; i < u_VPLsCount; ++i)
	{
		vec3 VPLRadianceIntensity = u_VPLs[i].RadianceIntensity;
		vec3 VPLNormalInViewSpaceAndAlphai = u_VPLs[i].NormalInViewSpaceAndAlphai;
		vec3 VPLPositionInViewSpace = u_VPLs[i].PositionInViewSpace.xyz;
		float VPLAlphai =  u_VPLs[i].Alphai;
		float VPLRadius = u_VPLs[i].Radius;

		IndirectIllumination += calcVPLIrradiance(VPLRadianceIntensity, VPLNormalInViewSpaceAndAlphai, VPLPositionInViewSpace, FragPos, FragNormal, VPLAlphai, VPLRadius, u_RandomNumbers[i]);
	}

	IndirectIllumination *= FragAlbedo;

	vec3 Result= DirectIllumination + IndirectIllumination;
	imageStore(u_OutputImage, uv, vec4(Result, 1.0));
}