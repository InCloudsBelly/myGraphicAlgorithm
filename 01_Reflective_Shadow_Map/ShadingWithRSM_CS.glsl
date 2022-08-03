#version 430 core
#pragma optionNV (unroll all)	//暂时不知道有没有起作用

#define LOCAL_GROUP_SIZE 16
#define VPL_NUM 256

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

layout(std140, binding = 1) uniform VPLsSampleCoordsAndWeights
{
	vec4 u_VPLsSampleCoordsAndWeights[VPL_NUM];
};

uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;
uniform sampler2D u_RSMFluxTexture;
uniform sampler2D u_RSMNormalTexture;		
uniform sampler2D u_RSMPositionTexture;
uniform sampler2D u_RSMDepthTexture;
uniform mat4  u_LightVPMatrix;
uniform float u_MaxSampleRadius;
uniform int   u_RSMSize;
uniform int   u_VPLNum;
uniform vec3  u_LightDir;

#define near_plane 0.5
#define far_plane 20
			
float LinerizeDepth(float depth)
{
	float z=depth*2.0-1.0;
	return (2.0*near_plane*far_plane)/(far_plane + near_plane - z * (far_plane - near_plane));
}

vec3 calcVPLIrradiance(vec3 vVPLFlux, vec3 vVPLNormal, vec3 vVPLPos, vec3 vFragPos, vec3 vFragNormal, float vWeight)
{
	vec3 VPL2Frag = normalize(vFragPos - vVPLPos);
	return vVPLFlux * max(dot(vVPLNormal, VPL2Frag), 0) * max(dot(vFragNormal, -VPL2Frag), 0)/(length(vFragPos - vVPLPos) * length(vFragPos - vVPLPos)) * vWeight;
}

void main()
{
	if(u_VPLNum != VPL_NUM)
		return;

	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	vec3 FragNormal = normalize(texelFetch(u_NormalTexture, uv, 0).xyz);
	vec3 FragAlbedo = texelFetch(u_AlbedoTexture, uv, 0).xyz;
	vec3 FragPos = texelFetch(u_PositionTexture, uv, 0).xyz;

	vec4 FragPosInLightSpace = u_LightVPMatrix * vec4(FragPos, 1);

	FragPosInLightSpace /= FragPosInLightSpace.w;

	vec3 FragNDCPos4Light = FragPosInLightSpace.xyz *0.5 + 0.5;

	float depthValue=texture(u_RSMDepthTexture, FragNDCPos4Light.xy).r;
	
	float shadow=FragNDCPos4Light.z-0.01>depthValue?0.1:1.0;


	float RSMTexelSize = 1.0 / u_RSMSize;
	vec3 DirectIllumination;
	if(FragNDCPos4Light.z < 0.0f || FragNDCPos4Light.x > 1.0f || FragNDCPos4Light.y > 1.0f || FragNDCPos4Light.x < 0.0f || FragNDCPos4Light.y < 0.0f )
		DirectIllumination = vec3(0.1) * FragAlbedo;
	else
		DirectIllumination = FragAlbedo * max(dot(-u_LightDir, FragNormal), 0.0);
	vec3 IndirectIllumination = vec3(0);
	for(int i = 0; i < u_VPLNum; ++i)
	{
		vec3 VPLSampleCoordAndWeight = u_VPLsSampleCoordsAndWeights[i].xyz;
		vec2 VPLSamplePos = FragNDCPos4Light.xy + u_MaxSampleRadius * VPLSampleCoordAndWeight.xy * RSMTexelSize;
		vec3 VPLFlux = texture(u_RSMFluxTexture, VPLSamplePos).xyz;
		vec3 VPLNormal = normalize(texture(u_RSMNormalTexture, VPLSamplePos).xyz);
		vec3 VPLPosition = texture(u_RSMPositionTexture, VPLSamplePos).xyz;

		IndirectIllumination += calcVPLIrradiance(VPLFlux, VPLNormal, VPLPosition, FragPos, FragNormal, VPLSampleCoordAndWeight.z);
	}
	IndirectIllumination *= FragAlbedo;

	vec3 Result = DirectIllumination * shadow  + IndirectIllumination / u_VPLNum;

	imageStore(u_OutputImage, uv, vec4(Result, 1.0));
}