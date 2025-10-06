cbuffer VS_CONSTANT_BUFFER0 : register(b0)
{
    float4x4 mtx; // •ÏŠ·‚µ‚½‚¢s—ñ‚ğ“ü‚ê‚é
}

cbuffer VS_CONSTANT_BUFFER1: register(b1)
{
    float4 position;
}

struct VS_INPUT
{
    float4 posL : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 posH : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

                    // «‚É’¸“_‚ª•Û‘¶
VS_OUT main( VS_INPUT vsin )
{
    VS_OUT vsout;
    
    vsout.posH = mul(vsin.posL, mtx);
    vsout.color = vsin.color;
    vsout.uv = vsin.uv;
    
    return vsout;  // mul‚ÍŠ|‚¯Z‚ÌˆÓ
}