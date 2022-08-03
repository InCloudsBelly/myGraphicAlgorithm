#version 430 core

#define LOCAL_GROUP_SIZE 16

layout (local_size_x = LOCAL_GROUP_SIZE, local_size_y = LOCAL_GROUP_SIZE) in;

uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_PositionTexture;

layout (rgba32f, binding = 0) uniform writeonly image2D u_OutputImage;

uniform int u_WindowWidth;
uniform int u_WindowHeight;
uniform int u_SubBufferNumX;
uniform int u_SubBufferNumY;
uniform int u_TotalLightNum;


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

vec3 calcPointLight(vec3 FragPos, vec3 Normal, vec3 LightPos, vec3 LightColor, float Radius)
{
	vec3 LightDir = FragPos - LightPos;
	float Distance = length(LightDir);
	LightDir = normalize(LightDir);

	float Attenuation = max(1.0f - Distance / Radius, 0.0f);
	return max(dot(-LightDir, Normal), 0.0f) * LightColor * Attenuation;
}

void main()
{
	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	ivec2 SubBufferSize = ivec2(u_WindowWidth / u_SubBufferNumX, u_WindowHeight / u_SubBufferNumY);
	int   SubBufferNum = u_SubBufferNumX * u_SubBufferNumY;
	int   LightNumPerSubBuffer = (u_TotalLightNum + SubBufferNum - 1) / SubBufferNum;

	ivec2 SubBufferIndex2 = ivec2(uv.x / SubBufferSize.x, uv.y / SubBufferSize.y);
	int   SubBufferIndex = SubBufferIndex2.y * u_SubBufferNumX + SubBufferIndex2.x;
	int   LightStartIndex = SubBufferIndex * LightNumPerSubBuffer;
	int   LightEndIndex = min(LightStartIndex + LightNumPerSubBuffer, u_TotalLightNum);

	//vec3 Albedo = texelFetch(u_AlbedoTexture, uv, 0).xyz;/此处不使用色彩纹理，因为后续需要高斯模糊，为避免将纹理也模糊掉，最后QuadPass再使用Albedo。
	vec3 Normal = texelFetch(u_NormalTexture, uv, 0).xyz;
	vec3 FragPos = texelFetch(u_PositionTexture, uv, 0).xyz;

	vec3 FragColor = vec3(0);
	for(int i = LightStartIndex; i < LightEndIndex; ++i)
	{
		_PointLight PointLight = u_PointLights[i];
		vec4 LightPos =  PointLight.Position;
		FragColor += calcPointLight(FragPos, Normal, LightPos.xyz, PointLight.ColorAndRadius.xyz, PointLight.ColorAndRadius.w);
	}
	FragColor *= u_SubBufferNumX * u_SubBufferNumY;

	imageStore(u_OutputImage, uv, vec4(FragColor, 1.0f));
}