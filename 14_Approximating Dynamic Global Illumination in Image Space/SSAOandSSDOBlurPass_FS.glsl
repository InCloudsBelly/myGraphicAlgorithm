#version 430 core

in  vec2 v2f_TexCoords;
layout (location = 0) out float FragColor_AO_;
layout (location = 1) out vec3 FragColor_DO_;

uniform sampler2D u_SSAOTexture;
uniform sampler2D u_SSDOTexture;


void main()
{
    vec2 TexelSize = 1.0 / vec2(textureSize(u_SSAOTexture, 0));
    float Result_AO = 0.0;
    vec3 Result_DO = vec3(0);

    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 Offset = vec2(float(x), float(y)) * TexelSize;
            Result_AO += texture(u_SSAOTexture, v2f_TexCoords + Offset).r;
            Result_DO += texture(u_SSDOTexture, v2f_TexCoords + Offset).rgb;
        }
    }
    FragColor_AO_ = Result_AO / (4.0 * 4.0);
    FragColor_DO_ = Result_DO / (4.0 * 4.0);
}