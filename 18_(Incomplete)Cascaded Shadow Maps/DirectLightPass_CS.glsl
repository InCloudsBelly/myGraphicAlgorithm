#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 32

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_InputAlbedoTexture;
uniform sampler2D u_InputNormalTexture;
uniform sampler2D u_InputPositionTexture;
uniform sampler2DArray u_LightDepthTexture;	

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputDirectIlluminationImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform float u_FarPlane[5];
uniform mat4  u_LightVPMatrix[5];
uniform vec3  u_LightDir;	
uniform vec3 u_CameraPos;
uniform float  u_Intensity;	
uniform int u_SpiltNum = 5;
void main()
{
	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	vec4 Albedo = texelFetch(u_InputAlbedoTexture, uv, 0);
	vec4 Normal = texelFetch(u_InputNormalTexture, uv, 0);
	vec4 Position = texelFetch(u_InputPositionTexture, uv, 0);
	vec4 FragNormalAndDoubleRoughness = Normal;	//对vec3(0)不能做归一化
	vec3 FragNormal = normalize(FragNormalAndDoubleRoughness.xyz);
	vec4 FragAlbedoAndMetallic = Albedo;
	vec3 FragPos = Position.xyz;
	vec3 ViewDir = normalize(u_CameraPos - FragPos);
	vec3 SpecularColor;
	float DirectIllumination;

	int index = 4;
	float currentDepth = (u_ViewMatrix * Position).z;
	if(currentDepth < u_FarPlane[0])
		index = 0;
	else if(currentDepth < u_FarPlane[1])
		index = 1;
	else if(currentDepth < u_FarPlane[2])
		index = 2;
	else if(currentDepth < u_FarPlane[3])
		index = 3;
		
	vec4 FragPosInLightSpace = u_LightVPMatrix[index] * vec4(FragPos, 1);
	FragPosInLightSpace /= FragPosInLightSpace.w;
	FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
	vec3 LightDir = normalize(u_LightDir);	//这个负号不要忘了
	float Visibility4DirectLight = 0.0f;
	if(FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f)
		DirectIllumination = 0.1;
	else
	{
		vec3 FragNDCPos4Light = vec3(FragPosInLightSpace.xy,index);
		float ClosetDepth4Light = texture(u_LightDepthTexture, FragNDCPos4Light).r; 
		float Bias = 0.005f;
		Visibility4DirectLight = (FragPosInLightSpace.z - Bias <= ClosetDepth4Light) ? 1.0f : 0.0f;
		DirectIllumination = /*u_Intensity * max(dot(-LightDir, FragNormal), 0) **/ Visibility4DirectLight + 0.1;
	}


	imageStore(u_OutputDirectIlluminationImage, uv, vec4(DirectIllumination,DirectIllumination,DirectIllumination, 1) * Albedo);
}