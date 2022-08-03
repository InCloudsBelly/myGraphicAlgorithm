#version 430 core

#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;
uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;

uniform int u_LowWidth;
uniform int u_LowHeight;
uniform int u_BlockX;
uniform int u_BlockY;
struct _PointLight
{
	vec4  Position;
	vec4  ColorAndRadius;
	vec4 Importance;
};

layout (std430, binding = 0) buffer PointLights
{
	_PointLight u_PointLights[];
};

layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

vec3 calcPointLight(vec3 vFragPos, vec3 vNormal, vec3 vLightPos, vec3 vLightColor, float vRadius)
{
	vec3 LightDir = vLightPos - vFragPos;
	float Distance = length(LightDir);
	LightDir = normalize(LightDir);
	float Attenuation = max(1.0f - Distance / vRadius, 0.0f);
	return max(dot(LightDir, vNormal), 0.0f) * vLightColor * Attenuation;
}

void main()
{
	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	int Index = int(u_BlockX * gl_GlobalInvocationID.y + gl_GlobalInvocationID.x);
	vec3 Sum = vec3(0);
	_PointLight PointLight = u_PointLights[Index];

	for(int i = 0 ; i < u_LowWidth ; i++)
		for(int j = 0 ; j < u_LowHeight ; j++)
		{	
			ivec2 TexCoord = ivec2(i,j);
			vec3 Normal = texelFetch(u_NormalTexture, TexCoord, 0).xyz;
			vec3 Albedo = texelFetch(u_AlbedoTexture, TexCoord, 0).xyz;
			vec3 FragPos = texelFetch(u_PositionTexture, TexCoord, 0).xyz;
			vec4 LightPos =  PointLight.Position;
			Sum += calcPointLight(FragPos, Normal, LightPos.xyz, PointLight.ColorAndRadius.xyz, PointLight.ColorAndRadius.w);
		}
//	Sum /= u_LowHeight*u_LowWidth;
	u_PointLights[Index].Importance = vec4(dot(Sum,Sum),1,0,0);
}