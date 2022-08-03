#version 430 core

in  vec3 v2f_FragPos;
in  vec2 v2f_TexCoords;
in  vec3 v2f_Normal;
layout(std140, binding = 0) uniform u_Matrices4ProjectionWorld
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};
layout (location = 0) out vec4 Albedo_;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;
uniform vec3 CameraPos;
uniform vec3 u_DiffuseColor;
uniform vec3 u_LightDir = vec3( 0.146059, -0.594963, -0.790371);

void main()
{	
	if((abs(v2f_Normal.x) < 0.0001f) && (abs(v2f_Normal.y) < 0.0001f) && (abs(v2f_Normal.z) < 0.0001f))
	{
		Albedo_ = vec4(0, 0, 0, 1);
		return;
	}
	vec3 Normal = normalize(v2f_Normal);
	vec3 LightDir= normalize(u_LightDir);
	vec3 ViewDir = -normalize(v2f_FragPos - CameraPos);

	vec3 diffuseColor = texture(u_DiffuseTexture, v2f_TexCoords).xyz;
	vec3 specularColor = texture(u_SpecularTexture, v2f_TexCoords).xyz;



	vec3 Cool = vec3(0,0,0.55) + 0.25 * diffuseColor;
	vec3 Warm = vec3(0.3,0.3,0) + 0.5 * diffuseColor;

	float t = (1 + max(0.0,dot(-LightDir,Normal))) / 2;
	vec3 R = reflect(LightDir, Normal.xyz);
	vec3 H = normalize(-LightDir + ViewDir);
	vec3 specular = specularColor * pow(max(0.0, dot(H, Normal)),16);

	Albedo_ = vec4( t * Warm + (1 - t) * Cool  + specular , 1.0);
}