#version 430 core
#pragma optionNV (unroll all)	

#define LOCAL_GROUP_SIZE 32

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_InputAlbedoTexture;
uniform sampler2D u_InputNormalTexture;
uniform sampler2D u_InputPositionTexture;
uniform sampler2D u_LightDepthTexture;	
uniform sampler2D u_BlurLightDepthTexture;	

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputDirectIlluminationImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

uniform mat4  u_LightVPMatrixMulInverseCameraViewMatrix;
uniform vec3  u_LightDir;	
uniform float  u_Intensity;	
uniform float  m_CameraSizeExtent;	

void main()
{
	ivec2 FragPos = ivec2(gl_GlobalInvocationID.xy);
	vec4 Normal = texelFetch(u_InputNormalTexture, FragPos, 0);
	vec4 Position = texelFetch(u_InputPositionTexture, FragPos, 0);
	vec3 Albedo = texelFetch(u_InputAlbedoTexture, FragPos, 0).xyz;
	vec3 FragViewNormal = normalize(Normal.xyz);
	vec3 FragViewPos = Position.xyz;

	vec4 FragPosInLightSpace = u_LightVPMatrixMulInverseCameraViewMatrix * vec4(FragViewPos, 1);
	vec3 LightDirInViewSpace = -normalize(vec3(u_ViewMatrix * vec4(u_LightDir, 0.0f)));	//������Ų�Ҫ����
	float DirectIllumination;
	FragPosInLightSpace /= FragPosInLightSpace.w;
	float radius = FragPosInLightSpace.x * FragPosInLightSpace.x+ FragPosInLightSpace.y * FragPosInLightSpace.y;

	FragPosInLightSpace.xyz = (FragPosInLightSpace.xyz + 1) / 2;
	float Visibility4DirectLight = 0.0f;
	if(FragPosInLightSpace.z < 0.0f || FragPosInLightSpace.x > 1.0f || FragPosInLightSpace.y > 1.0f || FragPosInLightSpace.x < 0.0f || FragPosInLightSpace.y < 0.0f || radius > m_CameraSizeExtent)
		DirectIllumination = 0;
	else
	{
		float Depth = (FragPosInLightSpace.z);
		vec2 FragNDCPos4Light = FragPosInLightSpace.xy;
		vec2 VarianceData  = texture(u_BlurLightDepthTexture, FragNDCPos4Light).rg; 
		float Var = VarianceData.g - VarianceData.r * VarianceData.r;
		if(Depth - 0.0001 < VarianceData.r){
			Visibility4DirectLight =  1.0;
		}
		else{
		    Visibility4DirectLight = Var /(Var + (Depth - VarianceData.r)*(Depth - VarianceData.r));
		}
		DirectIllumination = u_Intensity * max(dot(LightDirInViewSpace, FragViewNormal), 0) * Visibility4DirectLight;
	}

	imageStore(u_OutputDirectIlluminationImage, FragPos, vec4(DirectIllumination, DirectIllumination, DirectIllumination, 1));
}