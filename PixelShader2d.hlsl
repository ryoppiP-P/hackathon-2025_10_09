float4 color;

struct PS_INPUT
{
    float4 posL : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

Texture2D major_texture;    // テクスチャ
SamplerState major_sampler; // テクスチャサンプラ

float4 main(PS_INPUT psin) : SV_TARGET
{
    float4 texcolor = major_texture.Sample(major_sampler, psin.uv);
    
    return psin.color * texcolor * color;    // t.r * c.r, t.g * c.g, t.b * c.b, t.a * c.a
}