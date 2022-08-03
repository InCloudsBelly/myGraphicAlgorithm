#version 430 core

#define LOCAL_GROUP_SIZE 16
#define MAX_LIGHT_NUM_IN_SHARED_MEMORY 10000

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

struct SPointLight
{
	vec4  Position;
	vec4  ColorAndRadius;
};

layout (std430, binding = 0) buffer PointLights
{
	SPointLight u_PointLights[];
};

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform int u_TotalLightNum;
uniform int u_WindowWidth;
uniform int u_WindowHeight;

uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_DepthTexture;

uniform mat4  u_InverseProjectionMatrix;
uniform float u_NearPlane;
uniform float u_FarPlane;

shared uint s_MinDepth = 0xFFFFFFFF;
shared uint s_MaxDepth = 0;
shared uint s_CulledLightIndexs[MAX_LIGHT_NUM_IN_SHARED_MEMORY];
shared uint s_CulledLightCount = 0;

vec4 convertNDC2ViewSpace(vec4 vNDCCoord)
{
	vNDCCoord = u_InverseProjectionMatrix * vNDCCoord;
	vNDCCoord /= vNDCCoord.w;
	return vNDCCoord;
}

vec4 createPlane(vec4 vVector1, vec4 vVector2)
{
	vec4 Plane;
	Plane.xyz = normalize(cross(vVector1.xyz, vVector2.xyz));
	Plane.w = 0;	//��׶���ĸ�ƽ�涼���۲�ռ�ԭ�㣬���Ծ���dӦ����0
	return Plane;
}

float calcSignedDistanceFromPlane(vec4 vPoint, vec4 vPlane)	//����ƽ�涼�����۲�ռ�ԭ�㡣����Ļ�Ӧ���ǵ�˽����ȥƽ����۲�ռ�ԭ��ľ���
{
	return abs(dot(vPoint.xyz, vPlane.xyz));
}

float LinearizeDepth(float vDepth)
{
	float z = vDepth * 2.0 - 1.0; // Back to NDC 
	return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));
}

vec3 calcPointLight(vec3 vFragViewPos, vec3 vNormal, vec3 vLightViewPos, vec3 vLightColor, float vRadius)
{
	vec3 LightDir = vLightViewPos - vFragViewPos;
	float Distance = length(LightDir);
	LightDir = normalize(LightDir);
	float Attenuation = max(1.0f - Distance / vRadius, 0.0f);
	return max(dot(LightDir, vNormal), 0.0f) * vLightColor * Attenuation;
}

void main()
{
	//���м���Tile����С��������ֵ�����б���ԭ�Ӳ����������ķ���
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec2 TexCoord = vec2(float(FragPos.x) / u_WindowWidth, float(FragPos.y) / u_WindowHeight);
	float FragDepth = texture(u_DepthTexture, TexCoord).r;
	//�����ֵת��Ϊ�޷������ͣ���Щ��ȶ���С��1����0��
	uint FragDepthInt = uint(FragDepth * 0xFFFFFFFFu);
	atomicMin(s_MinDepth, FragDepthInt);	
	atomicMax(s_MaxDepth, FragDepthInt);
	
	barrier();							//�˴���barrier���Ա�֤���������е������̶߳���ɵ���һ���������������ȷ��������ֵ����Сֵ��


	float MaxDepthInViewSpace = LinearizeDepth(float(s_MaxDepth) / float(0xFFFFFFFFu));
	float MinDepthInViewSpace = LinearizeDepth(float(s_MinDepth) / float(0xFFFFFFFFu));

	//����Tile��׶����ĸ����棬��ʱ��
	uint TileMinX = LOCAL_GROUP_SIZE * gl_WorkGroupID.x;
	uint TileMaxX = LOCAL_GROUP_SIZE * (gl_WorkGroupID.x + 1);
	uint TileMinY = LOCAL_GROUP_SIZE * gl_WorkGroupID.y;
	uint TileMaxY = LOCAL_GROUP_SIZE * (gl_WorkGroupID.y + 1);
	vec4 TileCorners[4];
	TileCorners[0] = vec4((float(TileMinX) / u_WindowWidth) * 2.0f - 1.0f, (float(TileMinY) / u_WindowHeight) * 2.0f - 1.0f, 1.0f, 1.0f);
	TileCorners[1] = vec4((float(TileMaxX) / u_WindowWidth) * 2.0f - 1.0f, (float(TileMinY) / u_WindowHeight) * 2.0f - 1.0f, 1.0f, 1.0f);
	TileCorners[2] = vec4((float(TileMaxX) / u_WindowWidth) * 2.0f - 1.0f, (float(TileMaxY) / u_WindowHeight) * 2.0f - 1.0f, 1.0f, 1.0f);
	TileCorners[3] = vec4((float(TileMinX) / u_WindowWidth) * 2.0f - 1.0f, (float(TileMaxY) / u_WindowHeight) * 2.0f - 1.0f, 1.0f, 1.0f);
	TileCorners[0] = convertNDC2ViewSpace(TileCorners[0]);
	TileCorners[1] = convertNDC2ViewSpace(TileCorners[1]);
	TileCorners[2] = convertNDC2ViewSpace(TileCorners[2]);
	TileCorners[3] = convertNDC2ViewSpace(TileCorners[3]);
	vec4 TileFrustum[4];
	for(int i = 0; i < 4; ++i)
		TileFrustum[i] = createPlane(TileCorners[i], TileCorners[(i + 1) & 3]);

	//Tile��׶��͹�Դ�����ཻ���ԣ�������󽻿��ԸĽ�����AABB���ã�
	uint ThreadsNumInTile = LOCAL_GROUP_SIZE * LOCAL_GROUP_SIZE;
	for(uint i = 0; i < u_TotalLightNum; i += ThreadsNumInTile)	//i��ThreadsNumInTile��Ϊ���þֲ���������������̲߳��еĽ��й�Դ��Tile����
	{
		uint k = gl_LocalInvocationIndex + i;				//ÿ���̷ֱ߳���u_TotalLightNum /ThreadsNumInTile��256������Դ��һ��WorkGroup����256���̣߳������еؼ�����й�Դ��WorkGroup�����Tile�����󽻡� 
		if(k < u_TotalLightNum)
		{
			SPointLight PointLight = u_PointLights[k];
			vec4 LightViewPos = u_ViewMatrix * PointLight.Position;
			float LightRadius = PointLight.ColorAndRadius.w;
			if((-LightViewPos.z + LightRadius < MinDepthInViewSpace) || (-LightViewPos.z - LightRadius > MaxDepthInViewSpace))
				continue;
			//С�ڻ�������Դ��Tile��׶ƽ��ľ����Ǹ���
			if((calcSignedDistanceFromPlane(LightViewPos, TileFrustum[0]) < LightRadius) 
			&& (calcSignedDistanceFromPlane(LightViewPos, TileFrustum[1]) < LightRadius) 
			&& (calcSignedDistanceFromPlane(LightViewPos, TileFrustum[2]) < LightRadius) 
			&& (calcSignedDistanceFromPlane(LightViewPos, TileFrustum[3]) < LightRadius))
			{
				uint CurrrentCulledLightCount = atomicAdd(s_CulledLightCount, 1);
				s_CulledLightIndexs[CurrrentCulledLightCount] = k;
			}
		}
	}
	barrier();

	//������գ�ֱ�������Pass����ĺô��ǿɼ���Դ���������洢�������ڴ��ϣ����ô浽���Buffer�����棩����
	vec3 Normal = texture(u_NormalTexture, TexCoord).xyz;
	vec3 Albedo = texture(u_AlbedoTexture, TexCoord).xyz;
	vec3 FragViewPos = texture(u_PositionTexture, TexCoord).xyz;
	vec3 FragColor = Albedo * 0.1;
	if(FragDepth != 1.0f)
	{
		for(uint i = 0; i < s_CulledLightCount; ++i)
		{
			uint LightIndex = s_CulledLightIndexs[i];
			SPointLight PointLight = u_PointLights[LightIndex];
			vec4 LightViewPos = u_ViewMatrix * PointLight.Position;
			FragColor += calcPointLight(FragViewPos, Normal, LightViewPos.xyz, PointLight.ColorAndRadius.xyz, PointLight.ColorAndRadius.w);
		}
	}
	FragColor *= Albedo;
	//barrier();
	vec3 DebugColor;
	if(s_CulledLightCount <= 30)
		DebugColor = vec3(0, 0, 1);
	else if(s_CulledLightCount >30 && s_CulledLightCount < 50)
		DebugColor = vec3(0, 1, 0);
	else
		DebugColor = vec3(1, 0, 0);

	imageStore(u_OutputImage, FragPos, vec4(FragColor, 1.0f));
}